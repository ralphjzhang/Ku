/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <system_error>
#include "resolver.hpp"
#include "handle.hpp"
#include "common_ops.hpp"

namespace ku { namespace net {
  
class Endpoint;
namespace ops {
struct Socket;
} // namespace ku::net::ops

 
class StreamSocket
{
  typedef Handle<ops::Socket> HandleType;

public:
  explicit StreamSocket(HandleType&& handle) : handle_(std::move(handle)) { }
  StreamSocket(StreamSocket&&) = default;
  ~StreamSocket() = default;

  template <typename Buffer>
  inline ssize_t read(Buffer& buf, size_t count)
  { return ops::Common::read(handle_, buf, count); }

  template <typename Buffer>
  inline ssize_t write(Buffer const& buf, size_t count)
  { return ops::Common::write(handle_, buf, count); }

  explicit operator bool () const { return handle_.valid(); }
  HandleType const& handle() const { return handle_; }
  HandleType release_handle() { return std::move(handle_); }

protected:
  HandleType handle_;
};

class AcceptorSocket
{
  typedef Handle<ops::Socket> HandleType;

public:
  explicit AcceptorSocket(Endpoint const& endpoint);
  explicit AcceptorSocket(HandleType&& handle) : handle_(std::move(handle)) { }
  AcceptorSocket(AcceptorSocket&&) = default;
  ~AcceptorSocket() = default;

  HandleType const& handle() const { return handle_; }
  HandleType release_handle() { return std::move(handle_); }
  StreamSocket accept(Endpoint& endpoint);

private:
  void bind(Endpoint const& endpoint);
  void listen();

  HandleType handle_;
};

class ConnectorSocket : public StreamSocket // TODO no inheritance is better?
{
  typedef Handle<ops::Socket> HandleType;

public:
  ConnectorSocket(bool non_block = true);
  explicit ConnectorSocket(HandleType&& handle) : StreamSocket(std::move(handle)) { }
  ConnectorSocket(ConnectorSocket&&) = default;
  ~ConnectorSocket() = default;

  HandleType const& handle() const { return handle_; }
  void connect(Endpoint const& endpoint);
};

} } // namespace ku::net

