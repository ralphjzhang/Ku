#pragma once
#include <errno.h>
#include <system_error>

namespace ku { namespace fusion { namespace util {

inline std::error_code errc(int err)
{
  return std::make_error_code(static_cast<std::errc>(err));
}

inline std::error_code errc() { return errc(errno); }

template<typename To, typename From>
inline To implicit_cast(From const &f) { return f; }

struct noncopyable
{
  noncopyable() = default;
  ~noncopyable() = default;
  noncopyable(noncopyable const&) = delete;
  noncopyable& operator=(noncopyable const&) = delete;
};

} } } // namespace ku::fusion::util

