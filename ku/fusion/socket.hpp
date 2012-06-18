/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <system_error>
#include "handle.hpp"
#include "ops/common.hpp"

namespace ku { namespace fusion {
  
class SocketEndpoint;
namespace ops {
struct Socket;
} // namespace ku::fusion::ops

class Socket
{
  using HandleType = Handle<ops::Socket>;
  friend class AcceptorSocket;

public:
  Socket() = default;
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

protected:
  explicit Socket(HandleType&& handle) : handle_(std::move(handle)) { }
  HandleType handle_;
};

class AcceptorSocket
{
  using HandleType = Handle<ops::Socket>;

public:
  AcceptorSocket() = default;
  AcceptorSocket(AcceptorSocket&&) = default;
  ~AcceptorSocket() = default;

  HandleType const& handle() const { return handle_; }
  Socket accept(SocketEndpoint& endpoint);

  void bind_listen(SocketEndpoint const& endpoint);

private:
  HandleType handle_;
};

class ConnectorSocket : public Socket
{
  using HandleType = Handle<ops::Socket>;

public:
  ConnectorSocket() = default;
  ConnectorSocket(ConnectorSocket&&) = default;
  ~ConnectorSocket() = default;

  HandleType const& handle() const { return handle_; }
  void connect(SocketEndpoint const& endpoint, bool non_block = true);
};

} } // namespace ku::fusion

