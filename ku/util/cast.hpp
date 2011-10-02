#pragma once

namespace ku { namespace util {

template<typename To, typename From>
inline To implicit_cast(From const &f)
{
  return f;
}

} } // namespace ku::util

using ku::util::implicit_cast;

