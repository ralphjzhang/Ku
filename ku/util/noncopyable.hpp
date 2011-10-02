#pragma once

namespace ku { namespace util {

struct noncopyable
{
  noncopyable() = default;
  ~noncopyable() = default;

  noncopyable(noncopyable const&) = delete;
  noncopyable& operator=(noncopyable const&) = delete;
};

} } // namespace ku::util


