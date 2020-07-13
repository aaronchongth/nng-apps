#include <iostream>

#include <nngcore/Server.hpp>

int main(int argc, char** argv)
{
  std::string url = "udp://127.0.0.1:8889";
  Server::SharedPtr server = Server::make(std::move(url));

  if (!server)
    std::cout << "Client not available" << std::endl;
  server->test();

  return 0;
}
