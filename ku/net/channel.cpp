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

std::string to_str(Channel::Events evts)
{
  std::string s;
  if (evts.test(Channel::Close))
    s.append("Close");
  if (evts.test(Channel::Read))
    s.empty() ? s.append("Read") : s.append("|Read");
  if (evts.test(Channel::Write))
    s.empty() ? s.append("Write") : s.append("|Write");
  if (evts.test(Channel::Error))
    s.empty() ? s.append("Error") : s.append("|Error");
  return s;
}

} } // namespace ku::net

