#ifndef NNGCORE__INCLUDE__NNGCORE__SERVER_HPP
#define NNGCORE__INCLUDE__NNGCORE__SERVER_HPP

#include <string>
#include <memory>
#include <vector>
#include <functional>

#include <nng/nng.h>

class Server
{
public:

  struct Worker
  {
    enum
    {
      INIT,
      RECV
    } state;
    nng_aio* aio;
    nng_msg* msg;
    nng_ctx ctx;

    using Callback = void(void*);

    static Worker* make(nng_socket sock, Callback cb);
  };

  using SharedPtr = std::shared_ptr<Server>;

  static SharedPtr make(const std::string& url, int parallels = 128);

  void test();

  ~Server();

private:

  nng_socket _sock;
  std::vector<Worker*> _workers;

  static void callback_function(void* arg);

  Server();

};

#endif // NNGCORE__INCLUDE__NNGCORE__SERVER_HPP