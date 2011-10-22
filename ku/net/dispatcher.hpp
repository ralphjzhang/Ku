#pragma once
#include <system_error>
#include <functional>
#include "handle_ops.hpp"
#include "channel_ops.hpp"
#include "call_traits.hpp"

namespace ku { namespace net {

class ChannelHub;

// TODO parameterize EventHandler creation?
template <typename EventHandler>
void dispatch(Channel& chan, ChannelHub& hub)
{
  EventHandler& handler = chan.event_handler<EventHandler>();

  // Read
  if (chan.has_event(Channel::Read)) {
    if (chan.type() == Channel::Timer) {
      // Timer
      int64_t tick;
      read(chan, &tick, sizeof(tick));
      if (!util::if_handle_timer(handler, chan))
        hub.remove_channel(chan);
    } else if (chan.type() == Channel::Acceptor) {
      // Accept
      while (true) {
        Address addr;
        Channel conn_ch(accept(chan, addr));
        if (conn_ch.error()) {
          if (conn_ch.error() == std::errc::operation_would_block ||
              conn_ch.error() == std::errc::resource_unavailable_try_again) {
            // All incoming connections handled
            break;
          } else {
            if (!util::if_handle_error(handler, conn_ch))
              hub.remove_channel(chan);
            break;
          }
        } else if (util::if_handle_accept(handler, conn_ch, addr)) {
          conn_ch.set_event_handler(std::make_shared<EventHandler>());
          hub.adopt_channel(std::move(conn_ch));
        }
      }
    } else {
      // Data read
      assert(chan.type() == Channel::Connection);
      util::if_handle_read(handler, chan);
    }
  }
  // Write
  if (chan.has_event(Channel::Write))
    util::if_handle_write(handler, chan);
  // Error
  if (chan.has_event(Channel::Error))
    if (!util::if_handle_error(handler, chan))
      hub.remove_channel(chan);
  // Close
  if (chan.has_event(Channel::Close)) {
    util::if_handle_close(handler, chan);
    hub.remove_channel(chan);
  }
}


/**
 * A default dispatcher does nothing. Client code can choose to inherit from this calss, or
 * provide a complete implementation from scratch. A dispatcher must have following operations:
 * 
 *   bool on_setup(ChannelHub&);
 *   bool on_error(std::error_code);
 *   bool get_quit() const;
 *   void dispatch(Channel&, ChannelHub&);
 *
 * It is advised that dispatch calls the template method dispatch<EventHandler>, but client can
 * also choose to implement his own dispatching merchanism.
 **/
template <typename EventHandler>
class Dispatcher
{
public:
  Dispatcher() : quit_(false) { }

  // Dispatcher requirements
  bool on_setup(ChannelHub&) { return true; }
  bool on_error(std::error_code) { return false; }
  bool get_quit() const { return quit_; }
  void dispatch(Channel& chan, ChannelHub& hub)
  { return ku::net::dispatch<EventHandler>(chan, hub); }

  void quit() { quit_ = true; }

private:
  bool quit_;
};

} } // namespace ku::net

