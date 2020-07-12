#ifndef NNGCORE__INCLUDE__NNGCORE__SERVER_HPP
#define NNGCORE__INCLUDE__NNGCORE__SERVER_HPP

#include <stdio.h>
#include <string>
#include <memory>
#include <vector>

#include <nng/nng.h>
#include <nng/protocol/reqrep0/req.h>
#include <nng/supplemental/util/platform.h>

class Server
{
public:

  struct Work 
  {
    using Callback = std::function<void(void* arg)>;

    static Work* make(nng_socket sock, Callback cb);

    enum 
    { 
      INIT, 
      RECV, 
      WAIT, 
      SEND 
    } state;
    nng_aio* aio;
    nng_msg* msg;
    nng_ctx ctx;
  };

  using SharedPtr = std::shared_ptr<Server>;

  static SharedPtr make(const std::string& url, int parallels = 128);

  void test();

  ~Server();

private:

  nng_socket _sock;
  std::vector<Work*> _works;

  void callback_function(void* arg);

  Server();

};

#endif // NNGCORE__INCLUDE__NNGCORE__SERVER_HPP