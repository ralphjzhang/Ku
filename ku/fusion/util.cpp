#include <netinet/in.h>
#include <arpa/inet.h>
#include "util.hpp"

namespace ku { namespace fusion { namespace util {

size_t next_pow_of_two(size_t val)
{
  val |= val >> 1;
  val |= val >> 2;
  val |= val >> 4;
  val |= val >> 8;
  val |= val >> 16;
  val |= val >> 32;
  return ++val;
}

} } } // namespace ku::fusion::util
