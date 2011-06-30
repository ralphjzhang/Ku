#pragma once
#include <atomic>

namespace ku { namespace su {

struct sequence_generator
{
  std::atomic<size_t> value;

  sequence_generator( ) : value(0)
  { }
  sequence_generator( sequence_generator const& s ) = default;

  size_t next( )
  {
    return value++;
  }
};

size_t next_seq( )
{
  static std::atomic<size_t> value(0);
  return value++;
}

} } // namespace ku::su

