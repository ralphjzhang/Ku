#include <cassert>
#include <unistd.h>
#include "handle.hpp"

namespace ku { namespace net {

bool Handle::close()
{
  if (!valid())
    return true;
  if (::close(raw_handle_) == -1)
    set_error(errno);
  else
    raw_handle_ = 0;
  return !error();
}

} } // namespace ku::net

