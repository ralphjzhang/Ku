#pragma once
#include <system_error>
#include "handle.hpp"

struct addrinfo;

namespace ku { namespace net {
  
class Address;
 
/**
 * A Socket object owns a socket file descriptor upon creation.
 * Ownership can be transferred.
 **/
class Socket : public Handle
{
  friend Socket accept(Handle& h, Address& addr);

private:
  explicit Socket(int raw_handle) : Handle(raw_handle, true), listening_(false) { }

public:
  static Socket create(addrinfo const& addr);
  Socket(Socket&& s) : Handle(std::move(s)), listening_(s.listening_)
  { s.listening_ = false; };
  ~Socket() = default;

  bool listening() const { return listening_; }
  Socket& listen(Address const& addr);
  Socket accept(Address& addr) const;

private:
  bool set_non_block();

  bool listening_;
};

// Not all handles are applicable to accept()
Socket accept(Handle& h, Address& addr);

} } // namespace ku::net

