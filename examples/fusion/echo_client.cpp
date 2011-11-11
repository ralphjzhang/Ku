#include <iostream>
#include <sstream>
#include <ku/fusion/endpoint.hpp>
#include <ku/fusion/socket.hpp>

using namespace ku::fusion;

int main(int argc, char* argv[])
{
  if (argc < 2) {
    std::cout << "Usage: echo_server port endpoint" << std::endl;
    exit(0);
  }
  uint16_t port = atoi(argv[1]);
  Endpoint endpoint = argc > 2 ? Endpoint(argv[2], port) : Endpoint(port);

  try {
    ConnectorSocket socket(false);
    socket.connect(endpoint);

    for (int i = 0; i < 5; ++i) {
      std::stringstream ss;
      ss << "Message " << i + 1;
      std::cout << "Sending data: " << ss.str() << std::endl;
      socket.write(ss.str(), ss.str().length());
      char buf[64];
      ssize_t size = socket.read(buf, sizeof(buf));
      buf[size] = '\0';
      std::cout << "Get echo: " << buf << std::endl;
    }
  } catch (std::system_error const& ec) {
    std::cout << "Connect error in " << ec.what() << std::endl;
  }
}

