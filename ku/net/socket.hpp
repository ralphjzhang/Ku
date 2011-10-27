#pragma once
#include <system_error>
#include "resolver.hpp"
#include "handle.hpp"
#include "handle_ops.hpp"

namespace ku { namespace net {
  
class Endpoint;
 
class StreamSocket
{
public:
  explicit StreamSocket(Handle const& handle) : handle_(handle.raw_handle(), false) { }
  explicit StreamSocket(Handle&& handle) : handle_(std::move(handle)) { }
  StreamSocket(StreamSocket&&) = default;
  ~StreamSocket() = default;

  template <typename Buffer>
  inline ssize_t read(Buffer& buf, size_t count)
  { return ku::net::read(handle_, buf, count); }

  template <typename Buffer>
  inline ssize_t write(Buffer const& buf, size_t count)
  { return ku::net::write(handle_, buf, count); }

  int raw_handle() const { return handle_.raw_handle(); }
  Handle release_handle() { return std::move(handle_); }
  std::error_code const& error() const { return handle_.error(); }

protected:
  Handle handle_;
};


class AcceptorSocket
{
public:
  explicit AcceptorSocket(Endpoint const& endpoint);
  explicit AcceptorSocket(Handle const& handle) : handle_(handle.raw_handle(), false) { }
  explicit AcceptorSocket(Handle&& handle) : handle_(std::move(handle)) { }
  AcceptorSocket(AcceptorSocket&&) = default;
  ~AcceptorSocket() = default;

  StreamSocket accept(Endpoint& endpoint) { return StreamSocket(ku::net::accept(handle_, endpoint)); }
  int raw_handle() const { return handle_.raw_handle(); }
  Handle release_handle() { return std::move(handle_); }
  std::error_code const& error() const { return handle_.error(); }

private:
  bool listen(Endpoint const& endpoint)
  { return ku::net::bind(handle_, endpoint) && ku::net::listen(handle_); }

  Handle handle_;
};

class ConnectorSocket : public StreamSocket
{
public:
  ConnectorSocket();
  explicit ConnectorSocket(Handle const& handle) : StreamSocket(handle) { }
  explicit ConnectorSocket(Handle&& handle) : StreamSocket(std::move(handle)) { }
  ConnectorSocket(ConnectorSocket&&) = default;
  ~ConnectorSocket() = default;

  bool connect(Endpoint const& endpoint) { return ku::net::connect(handle_, endpoint); }
};

} } // namespace ku::net

