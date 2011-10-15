#include "sys.hpp"
#include "address.hpp"
#include "handle_ops.hpp"

namespace ku { namespace net {

Handle accept(Handle& h, Address& addr)
{
  auto ret = sys::accept(h.raw_handle(), addr);
  Handle handle(ret.first, true);
  handle.set_error(ret.second);
  if (!handle.error()) {
    handle.set_error(sys::set_non_block(handle.raw_handle()));
    if (!handle.error())
      handle.set_error(sys::set_close_exec(handle.raw_handle()));
  }
  return handle;
}

} } // namespace ku::net

