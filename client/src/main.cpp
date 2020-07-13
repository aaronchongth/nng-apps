#include <iostream>

#include <nngcore/Client.hpp>

int main(int argc, char** argv)
{
  std::string id_str(argv[1]);

  std::string url = "tcp://127.0.0.1:8889";
  Client::SharedPtr client = Client::make(std::move(url), std::atoi(id_str.c_str()));

  if (!client)
    std::cout << "Client not available" << std::endl;
  client->test();

  return 0;
}
