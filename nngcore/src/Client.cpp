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

  if ((rv = nng_listen(client->_sock, url.c_str(), NULL, 0)) < 0)
  {
    fatal("nng_listen", rv);
    return nullptr;
  }

  client->_id = id;
  return client;
}

void Client::test()
{
  int rv;

  for (;;)
  {
    free_fleet_msgs::FleetState fs;
    fs.set_name("test_fleet");
    free_fleet_msgs::RobotState* rs = fs.add_robots();
    rs->set_name("robot_" + std::to_string(_id));
    std::string serialized_fs = fs.SerializeAsString();
    
    std::size_t arr_size = serialized_fs.length() + 1;
    char char_array[arr_size];
    strcpy(char_array, serialized_fs.c_str());

    nng_msg* msg;
    if ((rv = nng_msg_alloc(&msg, 0)) != 0)
    {
      fatal("nng_msg_alloc", rv);
      continue;
    }
    if ((rv = nng_msg_append(msg, char_array, arr_size)) != 0)
    {
      fatal("nng_msg_append", rv);
      continue;
    }

    std::string topic_name = "fleet_state";
    std::size_t topic_name_size = topic_name.length() + 1;
    if ((rv = nng_msg_insert(msg, topic_name.c_str(), topic_name_size)) != 0)
    {
      fatal("nng_msg_insert", rv);
      continue;
    }

    std::cout << "client " << _id << " publishing serialized size: " 
        << arr_size - 1 << std::endl;

    if ((rv = nng_sendmsg(_sock, msg, nng_msg_len(msg))) != 0)
    {
      fatal("nng_sendmsg", rv);
      continue;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

Client::~Client()
{
  nng_close(_sock);
}

Client::Client()
{}
