#pragma once

namespace ku { namespace net {

class Channel;

class ChannelHub
{
public:
  virtual bool adopt_channel(Channel&& chan) = 0;
  virtual bool remove_channel(Channel const& chan) = 0;
  virtual bool modify_channel(Channel const& chan) = 0;

  ChannelHub() = default;
  virtual ~ChannelHub() { };
};

} } // namespace ku::net

