#include <cassert>
#include <unistd.h>
#include "handle.hpp"

namespace ku { namespace net {

void Handle::close()
{
  if (raw_handle_) {
    if (::close(raw_handle_) == -1)
      set_error(errno);
    else
      raw_handle_ = 0;
  }
}

void Handle::adopt(Handle&& h)
{
  // An owner adopting ownership of another Handle is a programming error.
  // If you really want to do that, release_handle() first.
  assert(!owner() && h.owner());
  raw_handle_ = h.raw_handle_;
  owner_ = true;
  error_ = h.error_;
  h.clear();
  assert(owner() && !h.owner());
}

} } // namespace ku::net
