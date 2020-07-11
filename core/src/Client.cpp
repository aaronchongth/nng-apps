#include "Client.hpp"
#include "utilities.hpp"

Client::SharedPtr Client::make(const std::string& url)
{
  SharedPtr client = SharedPtr(new Client);
  int rv;

	if ((rv = nng_req0_open(&client->_sock)) != 0) {
		fatal("nng_req0_open", rv);
    return nullptr;
	}

	if ((rv = nng_dial(client->_sock, url.c_str(), NULL, 0)) != 0) {
		fatal("nng_dial", rv);
    return nullptr;
	}

  return client;
}

Client::~Client()
{
  nng_close(_sock);
}

Client::Client()
{}
