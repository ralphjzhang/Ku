#include <iostream>
#include <ku/net/acceptor.hpp>
#include <ku/net/tcp_connection.hpp>
#include <ku/net/tcp_server.hpp>

using namespace ku::net;

class EchoHandler : public TCPConnection
{
public:
  EchoHandler(StreamSocket&& socket, Endpoint const& peer_endpoint) 
    : TCPConnection(std::move(socket), peer_endpoint)
  { }

  bool handle_read()
  {
    char buf[1024];
    ssize_t size = socket().read(buf, sizeof(buf));
    if (size > 0) {
      buf[size] = '\0';
      std::cout << "Client (" << to_str(peer_endpoint()) << ") sends message: " << buf << '\n';
      socket().write(buf, size); // TODO this is tricky, connection should handle this
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

  Endpoint endpoint("127.0.0.1", 8888);
  Acceptor<EchoHandler> acceptor(endpoint);
  TCPServer<decltype(acceptor)> server(acceptor);
  server.start();
  std::cout << "Server running, press enter to exit." << std::endl;
  std::getchar();
  server.stop();
  std::cout << "Server stopped, exiting program." << std::endl;
}

