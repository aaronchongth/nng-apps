#ifndef NNGCORE__INCLUDE__NNGCORE__SERVER_HPP
#define NNGCORE__INCLUDE__NNGCORE__SERVER_HPP

#include <string>
#include <memory>
#include <functional>

#include <nng/nng.h>

class Server
{
public:

  using SharedPtr = std::shared_ptr<Server>;

  static SharedPtr make(const std::string& url);

  void test();

  ~Server();

private:

  nng_socket _sock;

  Server();

};

#endif // NNGCORE__INCLUDE__NNGCORE__SERVER_HPP