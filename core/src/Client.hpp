#include <stdio.h>
#include <string>
#include <memory>

#include <nng/nng.h>
#include <nng/protocol/reqrep0/req.h>
#include <nng/supplemental/util/platform.h>

#include "free_fleet_msgs.pb.h"

class Client
{
public:

  using SharedPtr = std::shared_ptr<Client>;

  static SharedPtr make(const std::string& url);

  ~Client();

private:

  nng_socket _sock;
  nng_msg* msg;
  nng_time _start;
  nng_time _end

  Client();
};
