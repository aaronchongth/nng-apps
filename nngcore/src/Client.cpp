#include <thread>
#include <iostream>
#include <nngcore/Client.hpp>

#include <nng/protocol/pubsub0/pub.h>
#include <nng/protocol/pubsub0/sub.h>

#include "utilities.hpp"
#include "free_fleet_msgs.pb.h"

Client::SharedPtr Client::make(const std::string& url, int id)
{
  SharedPtr client = SharedPtr(new Client);
  int rv;

  if ((rv = nng_pub0_open(&client->_sock)) != 0)
  {
    fatal("nng_pub0_open", rv);
    return nullptr;
  }

  if ((rv = nng_listen(client->_sock, url.c_str(), nullptr, 0)) < 0)
  {
    fatal("nng_listen", rv);
    return nullptr;
  }

  client->_id = id;
  return client;
}

void Client::test()
{
  // nng_time start;
  // nng_time end;

  // start = nng_clock();

  for (;;)
  {
    free_fleet_msgs::FleetState fs;
    fs.set_name("test_fleet");
    free_fleet_msgs::RobotState* rs = fs.add_robots();
    rs->set_name("robot_" + std::to_string(_id));

    std::string serialized_fs = fs.SerializeAsString();

    nng_msg* msg;
    int rv;

    std::cout << "client " << _id << " publishing..." << std::endl;

    char* d = date();
    if ((rv == nng_send(_sock, d, strlen(d) + 1, 0)) != 0)
    // if ((rv = nng_send(
    //       _sock,
    //       static_cast<void*>(&serialized_fs),
    //       serialized_fs.c_str(),
    //       serialized_fs.size() + 1,
    //       0)) != 0)
    {
      fatal("nng_send", rv);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  // end = nng_clock();
	// printf("100 sends took %u milliseconds.\n", (uint32_t)(end - start));
}

Client::~Client()
{
  nng_close(_sock);
}

Client::Client()
{}
