#pragma once
#include <netinet/in.h>

namespace ku { namespace net {

class address
{
public:
  sockaddr_in const& sockaddr() const { return sockaddr_; }
  void set_sockaddr(sockaddr_in const& addr) { sockaddr_ = addr; }

private:
  sockaddr_in sockaddr_;
};

} } // namespace ku::net

