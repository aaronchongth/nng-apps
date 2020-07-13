#include <thread>
#include <iostream>

#include <nngcore/Server.hpp>

#include <nng/protocol/pubsub0/pub.h>
#include <nng/protocol/pubsub0/sub.h>

#include "utilities.hpp"
#include "free_fleet_msgs.pb.h"

Server::SharedPtr Server::make(const std::string& url)
{
  SharedPtr server = SharedPtr(new Server);
  int rv;

  if ((rv = nng_sub0_open(&server->_sock)) != 0)
  {
    fatal("nng_sub0_open", rv);
    return nullptr;
  }

  // subscribe to fleet states
  std::string topic = "fleet_state";
  char topic_char_array[topic.length() + 1];
  strcpy(topic_char_array, topic.c_str());
  topic_char_array[topic.length()] = '\0';

  if ((rv = nng_setopt(server->_sock, NNG_OPT_SUB_SUBSCRIBE, topic_char_array, topic.length() + 1)) != 0)
  {
    fatal("nng_setopt", rv);
    return nullptr;
  }
  if ((rv = nng_dial(server->_sock, url.c_str(), nullptr, 0)) != 0)
  {
    fatal("nng_dial", rv);
    return nullptr;
  }

  return server;
}

void Server::test()
{
  int rv;
  for (int i = 0; i < 1000000; ++i)
  {
    char* buf = nullptr;
    std::size_t sz;
    if ((rv = nng_recv(_sock, &buf, &sz, NNG_FLAG_ALLOC)) != 0)
    {
      fatal("nng_recv", rv);
    }

    std::string topic_name = "fleet_state";
    std::size_t topic_name_size = topic_name.size() + 1;

    if (sz < topic_name_size)
    {
      std::cout << "ERROR: Message seems to be shorter than the topic name" 
          << std::endl;
      continue;
    }

    char* msg_start = buf + topic_name_size;
    std::size_t msg_size = sz - topic_name_size - 1;
    std::string serialized_msg(msg_start, msg_size);

    // std::cout << "Serialized message size: " << msg_size << std::endl;

    free_fleet_msgs::FleetState fs;
    if (!fs.ParseFromString(serialized_msg))
    {
      std::cout << "Unable to deserialize message" << std::endl;
    }
    else
    {
      std::cout << "Received state with [" << fs.robots_size() << "] robots: ";
      for (int i = 0; i < fs.robots_size(); ++i)
      {
        std::cout << fs.robots(i).name() << ", ";
      }
      std::cout << std::endl;
    }

    nng_free(buf, sz);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

Server::Server()
{}

Server::~Server()
{}
