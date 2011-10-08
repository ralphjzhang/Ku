#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cerrno>
#include "address.hpp"
#include "socket.hpp"
#include "channel.hpp"
#include "epoll_poller.hpp"

namespace ku { namespace net {

Channel& Channel::operator = (Channel&& ch)
{
  if (this != &ch)
  {
    raw_handle_ = ch.raw_handle_;
    events_type_ = ch.events_type_;
    events_ = ch.events_;
    ch.clear();
  }
  return *this;
}

void Channel::handle_event()
{
  if ((events_ & EPOLLHUP) && !(events_ & EPOLLIN))
    ; // handle closed
  if (events_ & EPOLLERR)
    ; // handle error
  if (events_ & (EPOLLIN | EPOLLRDHUP))
    ; // handle read
  if (events_ & EPOLLOUT)
    ; // handle write
}

} } // namespace ku::net

