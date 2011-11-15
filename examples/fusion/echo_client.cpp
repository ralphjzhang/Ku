#include <iostream>
#include <sstream>
#include <ku/fusion/endpoint.hpp>
#include <ku/fusion/socket_endpoint.hpp>
#include <ku/fusion/socket.hpp>

using namespace ku::fusion;

int main(int argc, char* argv[])
{
  if (argc < 2) {
    std::cout << "Usage: echo_server endpoint" << std::endl;
    std::cout << "  e.g., echo_server tcp://*:8888" << std::endl;
    exit(0);
  }
  Endpoint ep(argv[1]);
  SocketEndpoint endpoint(ep);

  try {
    ConnectorSocket socket;
    socket.connect(endpoint, false);

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

