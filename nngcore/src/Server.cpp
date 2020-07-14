#include <thread>
#include <iostream>

#include <nngcore/Server.hpp>

#include <nng/protocol/pubsub0/pub.h>
#include <nng/protocol/pubsub0/sub.h>
#include <nng/protocol/reqrep0/rep.h>
#include <nng/supplemental/util/platform.h>

#include "utilities.hpp"
#include "free_fleet_msgs.pb.h"

Server::Worker* Server::Worker::make(
    nng_socket sock, Server::Worker::Callback cb)
{
  Worker* w;
  int rv;

  if ((w = static_cast<Worker*>(nng_alloc(sizeof(*w)))) == nullptr)
  {
    fatal("nng_alloc", NNG_ENOMEM);
    return nullptr;
  }
  if ((rv = nng_aio_alloc(&w->aio, cb, w)) != 0)
  {
    fatal("nng_aio_alloc", rv);
    return nullptr;
  }
  if ((rv = nng_ctx_open(&w->ctx, sock)) != 0)
  {
    fatal("nng_ctx_open", rv);
    return nullptr;
  }
  w->state = Worker::INIT;
  return w;
}

Server::SharedPtr Server::make(const std::string& url, int parallels)
{
  SharedPtr server = SharedPtr(new Server);
  int rv;

  if ((rv = nng_sub0_open(&server->_sock)) != 0)
  {
    fatal("nng_sub0_open", rv);
    return nullptr;
  }

  server->_workers.clear();
  for (int i = 0; i < parallels; ++i)
  {
    Worker* new_worker = Worker::make(server->_sock, callback_function);
    if (new_worker)
      server->_workers.push_back(new_worker);
  }

  // subscribe to fleet states
  std::string topic = "fleet_state";
  char topic_char_array[topic.length() + 1];
  strcpy(topic_char_array, topic.c_str());
  topic_char_array[topic.length()] = '\0';

  // if ((rv = nng_setopt(server->_sock, NNG_OPT_SUB_SUBSCRIBE, topic_char_array, topic.length() + 1)) != 0)
  if ((rv = nng_setopt(server->_sock, NNG_OPT_SUB_SUBSCRIBE, "", 0)) != 0)
  {
    fatal("nng_setopt", rv);
    return nullptr;
  }

  if ((rv = nng_listen(server->_sock, url.c_str(), nullptr, 0)) != 0)
  {
    fatal("nng_listen", rv);
    return nullptr;
  }

  // if ((rv = nng_dial(server->_sock, url.c_str(), nullptr, 0)) != 0)
  // {
  //   fatal("nng_dial", rv);
  //   return nullptr;
  // }

  return server;
}

void Server::test()
{
  for (Worker* w : _workers)
  {
    // this starts them going (INIT state)
    callback_function(w);
  }

  for (;;)
  {
    nng_msleep(3600000); // neither pause() nor sleep() portable
    // std::this_thread::sleep_for(std::chrono::milliseconds(3600000));
  }

  // int rv;
  // for (int i = 0; i < 1000000; ++i)
  // {
  //   char* buf = nullptr;
  //   std::size_t sz;
  //   if ((rv = nng_recv(_sock, &buf, &sz, NNG_FLAG_ALLOC)) != 0)
  //   {
  //     fatal("nng_recv", rv);
  //   }

  //   std::string topic_name = "fleet_state";
  //   std::size_t topic_name_size = topic_name.size() + 1;

  //   if (sz < topic_name_size)
  //   {
  //     std::cout << "ERROR: Message seems to be shorter than the topic name" 
  //         << std::endl;
  //     continue;
  //   }

  //   char* msg_start = buf + topic_name_size;
  //   std::size_t msg_size = sz - topic_name_size - 1;
  //   std::string serialized_msg(msg_start, msg_size);

  //   // std::cout << "Serialized message size: " << msg_size << std::endl;

  //   free_fleet_msgs::FleetState fs;
  //   if (!fs.ParseFromString(serialized_msg))
  //   {
  //     std::cout << "Unable to deserialize message" << std::endl;
  //   }
  //   else
  //   {
  //     std::cout << "Received state with [" << fs.robots_size() << "] robots: ";
  //     for (int i = 0; i < fs.robots_size(); ++i)
  //     {
  //       std::cout << fs.robots(i).name() << ", ";
  //     }
  //     std::cout << std::endl;
  //   }

  //   nng_free(buf, sz);
  //   std::this_thread::sleep_for(std::chrono::milliseconds(10));
  // }
}

Server::Server()
{}

Server::~Server()
{}

void Server::callback_function(void* arg)
{
  Worker* w = static_cast<Worker*>(arg);
  nng_msg* msg;
  int rv;

  switch(w->state)
  {
    case Worker::INIT:
    {
      w->state = Worker::RECV;
      nng_ctx_recv(w->ctx, w->aio);
      break;
    }
    case Worker::RECV:
    {
      if ((rv = nng_aio_result(w->aio)) != 0)
      {
        fatal("nng_ctx_recv", rv);
        break;
      }
      msg = nng_aio_get_msg(w->aio);
      std::cout << "Received a message of size: " << nng_msg_len(msg) 
          << std::endl;
      
      nng_msg_free(msg);
      nng_ctx_recv(w->ctx, w->aio);
      // if ((rv = nng_msg_trim(msg, nng_msg_len(msg))) != 0)
      // {

      // }
    }
    default:
    {
      fatal("bad state!", NNG_ESTATE);
      break;
    }
  }
}
