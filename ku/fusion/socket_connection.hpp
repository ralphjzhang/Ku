/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include "notice.hpp"
#include "endpoint.hpp"
#include "socket.hpp"

namespace ku { namespace fusion {

class SocketConnection
{
public:
  SocketConnection(StreamSocket&& socket, Endpoint const& peer_endpoint) 
    : socket_(std::move(socket)), peer_endpoint_(peer_endpoint)
  { }

  SocketConnection() = default; 

  StreamSocket& socket() { return socket_; }
  Endpoint const& peer_endpoint() const { return peer_endpoint_; }

private:
  StreamSocket socket_;
  Endpoint peer_endpoint_;
};

} } // namespace ku::fusion

