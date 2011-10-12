#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cerrno>
#include "address.hpp"
#include "sys.hpp"
#include "channel.hpp"

namespace ku { namespace net {

Channel& Channel::operator = (Channel&& ch)
{
  if (this != &ch)
  {
    raw_handle_ = ch.raw_handle_;
    event_types_ = ch.event_types_;
    events_ = ch.events_;
    error_ = ch.error_;
    ch.clear();
  }
  return *this;
}

int Channel::release_handle()
{
  int handle = raw_handle_;
  raw_handle_ = 0;
  return handle;
}

bool Channel::listen(Address const& addr)
{
  set_error(sys::bind(raw_handle(), addr));
  if (!error_) {
    set_error(sys::listen(raw_handle()));
    if (!error_) {
      set_event_type(Channel::Listen);
      return true;
    }
  }
  return false;
}

Channel Channel::accept(Address& addr)
{
  auto ret = sys::accept(raw_handle(), addr);
  Channel conn_chan(ret.first, Channel::In, ret.second);
  if (!conn_chan.error()) {
    conn_chan.set_error(sys::set_non_block(conn_chan.raw_handle()));
    if (!conn_chan.error())
      conn_chan.set_error(sys::set_close_exec(conn_chan.raw_handle()));
  }
  return std::move(conn_chan);
}

void Channel::clear()
{
  events_.reset();
  event_types_.reset();
  raw_handle_ = 0;
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

std::string to_str(Channel::EventTypes et)
{
  std::string s;
  if (et.test(Channel::Listen))
    s.append("Listen");
  if (et.test(Channel::In))
    s.empty() ? s.append("In") : s.append("|In");
  if (et.test(Channel::Out))
    s.empty() ? s.append("Out") : s.append("|Out");
  return s;
}

} } // namespace ku::net

