#pragma once

namespace ku { namespace net {

class Channel;

class ChannelHub
{
public:
  virtual bool adopt_channel(Channel&& ch) = 0;
  virtual bool remove_channel(int fd) = 0;
  virtual bool modify_channel(int fd, int event_types) = 0;

  ChannelHub() = default;
  virtual ~ChannelHub() { };
};

} } // namespace ku::net

