#pragma once
#include <system_error>

namespace ku { namespace net {

class ChannelHub;

class PollDispatcher
{
public:
  bool setup(ChannelHub& hub);
  bool quit();
  void dispatch(Channel& chan, ChannelHub& hub);

  bool on_error(std::error_code ec);
};

} } // namespace ku::net

