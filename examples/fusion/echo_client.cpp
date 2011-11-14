#include <iostream>
#include <sstream>
#include <ku/fusion/endpoint.hpp>
#include <ku/fusion/ip_endpoint.hpp>
#include <ku/fusion/tcp/socket.hpp>

using namespace ku::fusion;
using namespace ku::fusion::tcp;

int main(int argc, char* argv[])
{
  if (argc < 2) {
    std::cout << "Usage: echo_server endpoint" << std::endl;
    std::cout << "  e.g., echo_server tcp://*:8888" << std::endl;
    exit(0);
  }
  Endpoint ep(argv[1]);
  IPEndpoint endpoint(ep);

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

