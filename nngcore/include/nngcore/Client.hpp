#ifndef CORE__INCLUDE__CORE__CLIENT_HPP
#define CORE__INCLUDE__CORE__CLIENT_HPP

#include <stdio.h>
#include <string>
#include <memory>

#include <nng/nng.h>

class Client
{
public:

  using SharedPtr = std::shared_ptr<Client>;

  static SharedPtr make(const std::string& url, int id);

  void test();

  ~Client();

private:

  int _id;
  nng_socket _sock;

  Client();
};

#endif // CORE__INCLUDE__CORE__CLIENT_HPP
