#include <iostream>
#include <ku/net/acceptor.hpp>
#include <ku/net/tcp_connection.hpp>
#include <ku/net/tcp_server.hpp>

using namespace ku::net;

class EchoHandler : public TCPConnection
{
public:
  EchoHandler(StreamSocket&& socket, Address const& peer_address) 
    : TCPConnection(std::move(socket), peer_address)
  { }

  bool handle_read()
  {
    char buf[10];
    if (socket().read(buf, 10) > 0) {
      std::cout << "Get data from client: " << buf << std::endl;
      socket().write(buf, 6);
    }
    return true;
  }
};

int main(int argc, char* argv[])
{
  if (argc < 2) {
    std::cout << "Usage: echo_server port" << std::endl;
    exit(0);
  }

  Address addr("127.0.0.1", 8888);
  Acceptor<EchoHandler> acceptor(addr);
  TCPServer<decltype(acceptor)> server(acceptor);
  server.start();
  std::cout << "Server running, press enter to exit." << std::endl;
  std::getchar();
  server.stop();
  std::cout << "Server stopped, exiting program." << std::endl;
}

