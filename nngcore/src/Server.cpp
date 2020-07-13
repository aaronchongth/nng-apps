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
  if ((rv = nng_setopt(server->_sock, NNG_OPT_SUB_SUBSCRIBE, "fleet_state", 0)) != 0)
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
    std::cout << "received a message" << std::endl;
    nng_free(buf, sz);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

Server::Server()
{}

Server::~Server()
{}
