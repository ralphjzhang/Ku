/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <system_error>
#include "../handle.hpp"
#include "../ops/common.hpp"

namespace ku { namespace fusion {
  
class IPEndpoint;
namespace ops {
struct Socket;
} // namespace ku::fusion::ops

namespace tcp {
 
class Socket
{
  typedef Handle<ops::Socket> HandleType;

public:
  explicit Socket(HandleType&& handle) : handle_(std::move(handle)) { }
  Socket(Socket&&) = default;
  ~Socket() = default;

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
  explicit AcceptorSocket(IPEndpoint const& endpoint);
  explicit AcceptorSocket(HandleType&& handle) : handle_(std::move(handle)) { }
  AcceptorSocket(AcceptorSocket&&) = default;
  ~AcceptorSocket() = default;

  HandleType const& handle() const { return handle_; }
  HandleType release_handle() { return std::move(handle_); }
  Socket accept(IPEndpoint& endpoint);

private:
  void bind(IPEndpoint const& endpoint);
  void listen();

  HandleType handle_;
};

class ConnectorSocket : public Socket
{
  typedef Handle<ops::Socket> HandleType;

public:
  ConnectorSocket(bool non_block = true);
  explicit ConnectorSocket(HandleType&& handle) : Socket(std::move(handle)) { }
  ConnectorSocket(ConnectorSocket&&) = default;
  ~ConnectorSocket() = default;

  HandleType const& handle() const { return handle_; }
  void connect(IPEndpoint const& endpoint);
};

} } } // namespace ku::fusion::tcp

