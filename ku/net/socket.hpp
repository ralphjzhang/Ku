#pragma once
#include <system_error>
#include "addrinfo.hpp"
#include "handle.hpp"
#include "handle_ops.hpp"

namespace ku { namespace net {
  
class Address;
 
/**
 * A Socket object owns a socket file descriptor upon creation.
 * Ownership can be transferred.
 **/
class Socket : public Handle
{
public:
  explicit Socket(addrinfo const& addr);
  Socket(Socket&& s) : Handle(std::move(s)) { }
  ~Socket() = default;

  explicit Socket(Handle&& h) : Handle(std::move(h)) { }
};


class AcceptorSocket : public Socket
{
public:
  explicit AcceptorSocket(Address const& address) : Socket(AddrInfo())
  { listen(address); }

  Socket accept(Address& addr) { return Socket(ku::net::accept(*this, addr)); }

private:
  bool listen(Address const& addr)
  { return ku::net::bind(*this, addr) && ku::net::listen(*this); }
};

class StreamSocket : public Socket
{
public:
  template <typename Buffer>
  inline ssize_t read(Buffer&& buf, size_t count)
  { return ku::net::read(*this, buf, count); }


  template <typename Buffer>
  inline ssize_t write(Buffer& buf, size_t count)
  { return ku::net::write(*this, buf, count); }
};

} } // namespace ku::net

