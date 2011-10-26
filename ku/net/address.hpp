#pragma once
#include <netinet/in.h>
#include <string>

namespace ku { namespace net {

class Address
{
  friend std::string to_str(Address const&);

public:
  Address() = default;
  Address(uint16_t port);
  Address(std::string const& ip, uint16_t port);
  Address(Address const&) = default;
  ~Address() = default;

  sockaddr_in const& sockaddr() const { return sockaddr_; }
  sockaddr_in& sockaddr() { return sockaddr_; }
  void set_sockaddr(sockaddr_in const& addr) { sockaddr_ = addr; }

  std::string ip() const;
  uint16_t port() const;

private:
  sockaddr_in sockaddr_;
};

std::string to_str(Address const& addr);

} } // namespace ku::net

