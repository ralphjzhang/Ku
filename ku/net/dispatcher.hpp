#pragma once
#include <system_error>
#include <functional>
#include "call_traits.hpp"

namespace ku { namespace net {

class ChannelHub;

template <typename EventHandler>
void dispatch(Channel& chan, ChannelHub& hub)
{
  EventHandler& handler = chan.event_handler<EventHandler>();

  if (chan.has_event(Channel::Read)) {
    if (chan.type() == Channel::Timer) {
      util::if_handle_timer(handler, chan);
      int64_t tick;
      read(chan, &tick, sizeof(tick));
    } else if (chan.type() == Channel::Acceptor) {
      // Handle incoming connections
      while (true) {
        Address addr;
        Channel conn_ch(accept(chan, addr));
        if (conn_ch.error()) {
          if (conn_ch.error() == std::errc::operation_would_block ||
              conn_ch.error() == std::errc::resource_unavailable_try_again) {
            // All incoming connections handled
            break;
          } else {
            util::if_handle_error(handler, conn_ch);
            break;
          }
        } else if (util::if_handle_accept(handler, conn_ch, addr)) {
          conn_ch.set_event_handler(std::make_shared<EventHandler>());
          hub.adopt_channel(std::move(conn_ch));
        }
      }
    } else {
      assert(chan.type() == Channel::Connection);
      util::if_handle_read(handler, chan);
    }
  }
  if (chan.has_event(Channel::Write))
    util::if_handle_write(handler, chan);
  if (chan.has_event(Channel::Error))
    util::if_handle_error(handler, chan);
  if (chan.has_event(Channel::Close))
    util::if_handle_close(handler, chan, hub);
}


template <typename EventHandler>
class Dispatcher
{
public:
  // Dispatcher requirements
  std::function<bool(ChannelHub&)> on_setup;
  std::function<bool(std::error_code)> on_error;

  bool get_quit() const { return quit_; }
  void dispatch(Channel& chan, ChannelHub& hub) { return ku::net::dispatch<EventHandler>(chan, hub); }

  Dispatcher() : quit_(false)
  {
    // Client code can(should) override these default callbacks
    on_setup = [](ChannelHub&) { return true; };
    on_error = [](std::error_code) { return false; };
  }

  void quit() { quit_ = true; }

private:
  bool quit_;
};

} } // namespace ku::net

