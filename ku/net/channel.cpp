#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cerrno>
#include "address.hpp"
#include "socket.hpp"
#include "channel.hpp"
#include "poller.hpp"

namespace ku { namespace net {

void channel::handle_event()
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

