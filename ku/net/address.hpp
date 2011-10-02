#pragma once
#include <netinet/in.h>
#include <strings.h>
#include <string>
#include "util.hpp"

namespace ku { namespace net {

class address
{
public:
  address() = default;
  address(std::string const& ip, uint16_t port) { util::make_sockaddr(ip.c_str(), port, sockaddr_); }

  sockaddr_in const& sockaddr() const { return sockaddr_; }
  sockaddr_in& sockaddr() { return sockaddr_; }
  void set_sockaddr(sockaddr_in const& addr) { sockaddr_ = addr; }

private:
  sockaddr_in sockaddr_;
};

} } // namespace ku::net

