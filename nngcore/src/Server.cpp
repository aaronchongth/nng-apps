#include <nngcore/Server.hpp>

#include "utilities.hpp"
#include "free_fleet_msgs.pb.h"

Server::Work* Server::Work::make(nng_socket sock, Callback cb)
{
  Work* w;
  int rv;

	if ((w = static_cast<Work*>(nng_alloc(sizeof(*w)))) == nullptr)
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
	w->state = work::INIT;
	return w;
}

Server::SharedPtr Server::make(const std::string& url, int parallels)
{
  SharedPtr server = SharedPtr(new Server);
  int rv;

	rv = nng_rep0_open(&(server->_sock));
	if (rv != 0)
  {
		fatal("nng_rep0_open", rv);
    return nullptr;
	}
  
  for (int i = 0; i < parallels; ++i)
  {
    Work* new_work = Work::make(server->_sock);
    if (new_work)
      server->_works.push_back(new_work);
  }
}

void Server::test()
{}

Server::Server()
{}

Server::~Server()
{}

void Server::callback_function(void* arg)
{}
