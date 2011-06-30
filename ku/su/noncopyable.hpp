#pragma once

namespace ku { namespace su {

struct noncopyable
{
  noncopyable() = default;
  ~noncopyable() = default;

  noncopyable(noncopyable const&) = delete;
  noncopyable& operator=(noncopyable const&) = delete;
};

} } // namespace ku::su


