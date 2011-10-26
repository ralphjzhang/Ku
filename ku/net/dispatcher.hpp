#pragma once
#include <system_error>
#include <functional>
#include "handle_ops.hpp"
#include "channel_ops.hpp"
#include "call_traits.hpp"
#include "socket.hpp"

namespace ku { namespace net {

class ChannelHub;

template <typename EventHandler, typename Acceptor>
void accept_connections(Acceptor& acceptor, Channel& chan, ChannelHub& hub)
{
  /*
  while (true) {
    Address addr;
    AcceptorSocket acceptor_socket(chan.handle());
    StreamSocket conn_socket = acceptor_socket.accept(addr);
    if (acceptor_socket.error()) {
      if (acceptor_socket.error() == std::errc::operation_would_block ||
          acceptor_socket.error() == std::errc::resource_unavailable_try_again) {
        // All incoming connections handled
        break;
      } else {
        // Acceptor error
        if (!util::if_handle_error(acceptor, acceptor_socket))
          hub.remove_channel(chan);
        break;
      }
    } else {
      if (util::if_handle_accept(acceptor, conn_socket, addr)) {
        Channel conn_chan;
        conn_chan.adopt(std::move(conn_socket));
        conn_chan.set_event_type(Channel::In);
        conn_chan.set_event_handler(std::make_shared<EventHandler>());
        hub.adopt_channel(std::move(conn_chan));
      }
    }
  }
  */
}

template <typename Acceptor, typename Connection>
void dispatch(Channel& chan, ChannelHub& hub)
{
  Connection& connection = chan.event_handler<Connection>();

  // Read
  if (chan.has_event(Channel::Read)) {
    if (chan.type() == Channel::Acceptor) {
      // Accept
      chan.event_handler<Acceptor>().handle_accept(hub);
    } else if (chan.type() == Channel::Connection) {
      // Data read
      util::if_handle_read(connection);
    }
  }
  // Write
  if (chan.has_event(Channel::Write))
    util::if_handle_write(connection);
  // Error
  if (chan.has_event(Channel::Error))
    if (!util::if_handle_error(connection))
      hub.remove_channel(chan);
  // Close
  if (chan.has_event(Channel::Close)) {
    util::if_handle_close(connection);
    hub.remove_channel(chan);
  }
}

template <typename Acceptor, typename Connection, typename TimerHandler>
void dispatch(Channel& chan, ChannelHub& hub)
{
  // Timer
  if (chan.type() == Channel::Timer && chan.has_event(Channel::Read)) {
    int64_t tick;
    ::read(chan.raw_handle(), &tick, sizeof(tick));
    TimerHandler& timer_handler = chan.event_handler<TimerHandler>();
    if (!util::if_handle_timer_tick(timer_handler))
      hub.remove_channel(chan);
  }
  dispatch<Acceptor, Connection>(chan, hub);
}


/**
 * A default dispatcher does nothing. Client code can choose to inherit from this class, or
 * provide a complete implementation from scratch. A dispatcher must have following operations:
 * 
 *   bool initialize(ChannelHub&);
 *   bool on_error(std::error_code);
 *   bool get_quit() const;
 *   void dispatch(Channel&, ChannelHub&);
 *
 * It is advised that dispatch calls the template methods dispatch<>, but client can
 * also choose to implement his own dispatching merchanism.
 **/
template <typename EventHandler>
class Dispatcher
{
public:
  Dispatcher() : quit_(false) { }

  // Dispatcher requirements
  bool initialize(ChannelHub&) { return true; }
  bool on_error(std::error_code) { return false; }
  bool get_quit() const { return quit_; }
  void dispatch(Channel& chan, ChannelHub& hub) { }

  void quit() { quit_ = true; }

private:
  bool quit_;
};

} } // namespace ku::net

