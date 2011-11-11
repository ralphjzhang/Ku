/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include "socket_connection.hpp"
#include "call_selector.hpp"

namespace ku { namespace fusion {

template <typename EventHandler>
class ServerConnection : public SocketConnection
{
public:
  ServerConnection(StreamSocket&& socket, Endpoint const& peer_endpoint) 
    : SocketConnection(std::move(socket), peer_endpoint)
  { }

  ~ServerConnection() = default;

  bool operator()(Notice::Event event, NoticeId id)
  {
    switch(event)
    {
    case Notice::Read:
      util::if_handle_inbound(handler_, *this);
      break;
    case Notice::Write:
      util::if_handle_outbound(handler_, *this);
      break;
    case Notice::Error:
      if (!util::if_handle_error(handler_, *this)) {
        close();
        return false;
      }
      break;
    case Notice::Close:
      util::if_handle_close(handler_, *this);
      close();
      // After this, the notice is removed by poller
      break;
    default:
      assert(false);
    }
    return true;
  }

private:
  void close() { delete this; }

private:
  EventHandler handler_;
};

} } // namespace ku::fusion

