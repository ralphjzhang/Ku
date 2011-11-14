/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include "../notice.hpp"
#include "../ip_endpoint.hpp"
#include "socket.hpp"

namespace ku { namespace fusion { namespace tcp {

class SocketConnection
{
public:
  SocketConnection(Socket&& socket, IPEndpoint const& peer_endpoint) 
    : socket_(std::move(socket)), peer_endpoint_(peer_endpoint)
  { }

  SocketConnection() = default; 

  Socket& socket() { return socket_; }
  IPEndpoint const& peer_endpoint() const { return peer_endpoint_; }

private:
  Socket socket_;
  IPEndpoint peer_endpoint_;
};

} } } // namespace ku::fusion::tcp

