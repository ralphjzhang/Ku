#pragma once

namespace ku { namespace net {

template <typename Poller, typename EventHandler>
void loop(Poller& poller, EventHandler eh)
{
  while (1) {
    poller.poll(poller.events(), std::chrono::milliseconds(3000));
    if (poller.error())
      std::cout << poller.error().message() << std::endl; // handle poller error
    dispatch(poller.events(), eh);
  }
}

} } // namespace ku::net

