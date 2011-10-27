#include <iostream>
#include <sstream>
#include <ku/net/endpoint.hpp>
#include <ku/net/socket.hpp>

using namespace ku::net;

int main(int argc, char* argv[])
{
  if (argc < 2) {
    std::cout << "Usage: echo_server port endpoint" << std::endl;
    exit(0);
  }
  uint16_t port = atoi(argv[1]);
  Endpoint endpoint = argc > 2 ? Endpoint(argv[2], port) : Endpoint(port);

  ConnectorSocket socket;
  if (!socket.connect(endpoint)) {
    std::cout << "Connect error: " << socket.error().message() << std::endl;
    exit(1);
  }

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
}

