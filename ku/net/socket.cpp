#include <strings.h>
#include <sys/epoll.h>
#include <cerrno>
#include "sys.hpp"
#include "address.hpp"
#include "socket.hpp"

namespace ku { namespace net {

Socket Socket::create(addrinfo const& addr)
{
  Socket socket(::socket(addr.ai_family, addr.ai_socktype, addr.ai_protocol));
  if (socket.raw_handle() == -1) {
    socket.set_error(errno);
    return socket;
  }
  socket.set_non_block();
  return socket;
}

Socket& Socket::listen(Address const& addr)
{
  set_error(sys::bind(raw_handle(), addr));
  if (!error()) {
    set_error(sys::listen(raw_handle()));
    if (!error())
      listening_ = true;
  }
  return *this;
}

Socket Socket::accept(Address& addr) const
{
  auto ret = sys::accept(raw_handle(), addr);
  Socket socket(ret.first);
  socket.set_error(ret.second);
  socket.set_non_block();
  return socket;
}

bool Socket::set_non_block()
{
  set_error(sys::set_non_block(raw_handle()));
  if (!error()) {
    set_error(sys::set_close_exec(raw_handle()));
    return !error();
  }
  return false;
}


Socket accept(Handle& h, Address& addr)
{
  auto ret = sys::accept(h.raw_handle(), addr);
  Socket socket(ret.first);
  socket.set_error(ret.second);
  socket.set_non_block();
  return socket;
}

} } // namespace ku::net


