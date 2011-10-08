#pragma once

namespace ku { namespace net {

template <typename Poller, typename Events, typename EventHandler>
void loop(Poller& poller, Events& evts, EventHandler eh)
{
  while (1) {
    poller.poll(evts, std::chrono::milliseconds(100000));
    if (poller.error())
      ; // handle poller error
    dispatch(evts, eh);
  }
}

} } // namespace ku::net

