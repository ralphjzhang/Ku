#include "address.hpp"
#include "channel.hpp"

namespace ku { namespace net {

std::atomic<ChannelId> Channel::next_channel_id(0);

Channel& Channel::operator = (Channel&& chan)
{
  if (this != &chan)
  {
    event_handler_ = chan.event_handler_;
    raw_handle_ = chan.raw_handle_;
    id_ = chan.id_;
    type_ = chan.type_;
    event_types_ = chan.event_types_;
    events_ = chan.events_;
    chan.clear();
  }
  return *this;
}

void Channel::clear()
{
  event_handler_ = nullptr;
  raw_handle_ = 0;
  id_ = 0;
  type_ = Type::None;
  events_.reset();
  event_types_.reset();
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
  if (et.test(Channel::In))
    s.append("In");
  if (et.test(Channel::Out))
    s.empty() ? s.append("Out") : s.append("|Out");
  return s;
}

} } // namespace ku::net

