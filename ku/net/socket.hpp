#pragma once
#include <system_error>
#include "addrinfo.hpp"
#include "handle.hpp"
#include "handle_ops.hpp"

namespace ku { namespace net {
  
class Address;
 
class StreamSocket
{
public:
  explicit StreamSocket(Handle const& handle) : handle_(handle.raw_handle(), false) { }
  explicit StreamSocket(Handle&& handle) : handle_(std::move(handle)) { }
  StreamSocket(StreamSocket&&) = default;
  ~StreamSocket() = default;

  template <typename Buffer>
  inline ssize_t read(Buffer&& buf, size_t count)
  { return ku::net::read(handle_, buf, count); }

  template <typename Buffer>
  inline ssize_t write(Buffer& buf, size_t count)
  { return ku::net::write(handle_, buf, count); }

  int raw_handle() const { return handle_.raw_handle(); }
  Handle release_handle() { return std::move(handle_); }
  std::error_code const& error() const { return handle_.error(); }

private:
  Handle handle_;
};


class AcceptorSocket
{
public:
  explicit AcceptorSocket(Address const& address);
  explicit AcceptorSocket(Handle const& handle) : handle_(handle.raw_handle(), false) { }
  explicit AcceptorSocket(Handle&& handle) : handle_(std::move(handle)) { }
  AcceptorSocket(AcceptorSocket&&) = default;
  ~AcceptorSocket() = default;

  StreamSocket accept(Address& addr) { return StreamSocket(ku::net::accept(handle_, addr)); }
  int raw_handle() const { return handle_.raw_handle(); }
  Handle release_handle() { return std::move(handle_); }
  std::error_code const& error() const { return handle_.error(); }

private:
  bool listen(Address const& addr)
  { return ku::net::bind(handle_, addr) && ku::net::listen(handle_); }

  Handle handle_;
};

} } // namespace ku::net

