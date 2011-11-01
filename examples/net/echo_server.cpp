#include <iostream>
#include <ku/net/socket_acceptor.hpp>
#include <ku/net/server_connection.hpp>
#include <ku/net/tcp_server.hpp>

using namespace ku::net;

// ======================================================================================
// It's not really necessary to inherit from TCPConnection, as long as the handle provides
// the handle methods
// ======================================================================================
class EchoHandler
{
public:
  EchoHandler() = default;

  bool handle_inbound(SocketConnection& connection)
  {
    char buf[1024];
    ssize_t size = connection.socket().read(buf, sizeof(buf));
    if (size > 0) {
      buf[size] = '\0';
      std::cout << "Client (" << to_str(connection.peer_endpoint()) << ") sends message: "
        << buf << '\n';
      connection.socket().write(buf, size);
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

  Endpoint local_endpoint("127.0.0.1", 8888);
  TCPServer<EchoHandler> server(local_endpoint);
  server.start();
  std::cout << "Server running, press enter to exit." << std::endl;
  std::getchar();
  server.stop();
  std::cout << "Server stopped, exiting program." << std::endl;
}

