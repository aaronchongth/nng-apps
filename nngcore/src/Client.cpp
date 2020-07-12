#include <nngcore/Client.hpp>

#include "utilities.hpp"
#include "free_fleet_msgs.pb.h"

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

void Client::test()
{
  nng_time start;
  nng_time end;

  start = nng_clock();

  for (int i = 0; i < 100; ++i)
  {
    free_fleet_msgs::FleetState fs;
    fs.set_name("test_fleet");
    free_fleet_msgs::RobotState* rs = fs.add_robots();
    rs->set_name("test_robot");

    std::string serialized_fs = fs.SerializeAsString();

    nng_msg* msg;
    int rv;
    if ((rv = nng_msg_alloc(&msg, 0)) != 0)
    {
      fatal("nng_msg_alloc", rv);
    }
    if ((rv = nng_msg_append(
          msg, 
          static_cast<void*>(&serialized_fs), 
          serialized_fs.size())) != 0)
    {
      fatal("nng_msg_append", rv);
    }

    if ((rv = nng_sendmsg(_sock, msg, 0)) != 0)
    {
      fatal("nng_send", rv);
    }

    nng_msg_free(msg);
  }

  end = nng_clock();
	printf("100 sends took %u milliseconds.\n", (uint32_t)(end - start));
}

Client::~Client()
{
  nng_close(_sock);
}

Client::Client()
{}
