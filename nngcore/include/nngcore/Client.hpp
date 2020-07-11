#ifndef CORE__INCLUDE__CORE__CLIENT_HPP
#define CORE__INCLUDE__CORE__CLIENT_HPP

#include <stdio.h>
#include <string>
#include <memory>

#include <nng/nng.h>
#include <nng/protocol/reqrep0/req.h>
#include <nng/supplemental/util/platform.h>

class Client
{
public:

  using SharedPtr = std::shared_ptr<Client>;

  static SharedPtr make(const std::string& url);

  void test();

  ~Client();

private:

  nng_socket _sock;
  nng_msg* msg;

  Client();
};

#endif // CORE__INCLUDE__CORE__CLIENT_HPP
