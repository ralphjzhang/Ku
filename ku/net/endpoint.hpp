#pragma once
#include <netinet/in.h>
#include <string>

namespace ku { namespace net {

class Endpoint
{
  friend std::string to_str(Endpoint const&);

public:
  Endpoint() = default;
  Endpoint(uint16_t port);
  Endpoint(std::string const& ip, uint16_t port);
  Endpoint(Endpoint const&) = default;
  ~Endpoint() = default;

  sockaddr_in const& sockaddr() const { return sockaddr_; }
  sockaddr_in& sockaddr() { return sockaddr_; }
  void set_sockaddr(sockaddr_in const& addr) { sockaddr_ = addr; }

  std::string ip() const;
  uint16_t port() const;

private:
  sockaddr_in sockaddr_;
};

std::string to_str(Endpoint const& addr);

} } // namespace ku::net

