#pragma once
#include <type_traits>
#include <algorithm>
#include <cstring>
#include <ku/yuan/enable_if.hpp>

namespace ku { namespace chuan {

using ku::yuan::enable_if_c;
using ku::yuan::enable_if;

/// find number of digits (power of 10) in an UNSIGNED integral number
//  for 32-bit or shorter
template <typename T>
typename enable_if_c<(sizeof(T) <= 4), int>::type
digits( T t, typename enable_if<std::is_unsigned<T>>::type* = 0 )
{
  return
    (t >= 1e4)
    ? (t >= 1e7) 
      ? (t >= 1e9) ? 10 : (t >= 1e8) ? 9 : 8
      : (t >= 1e6) ? 7  : (t >= 1e5) ? 6 : 5
    : (t >= 1e2)
      ? (t >= 1e3) ? 4 : 3
      : (t >= 10)  ? 2 : 1;
}

/// find number of digits (power of 10) in an UNSIGNED integral number
//  for 64-bit
template <typename T>
typename enable_if_c<(sizeof(T) > 4), int>::type
digits( T t, typename enable_if<std::is_unsigned<T>>::type* = 0 )
{
  return (t <= 0xFFFFFFFF) ? digits(uint32_t(t)) :
    (t >= 1e14)
    ? (t >= 1e17)
      ? (t >= 1e19) ? 20 : (t >= 1e18) ? 19 : 18
      : (t >= 1e16) ? 17 : (t >= 1e15) ? 16 : 15
    : (t >= 1e12)
      ? (t >= 1e13) ? 14 : 13
      : (t >= 1e11) ? 12 : (t >= 1e10) ? 11 : 10;
}

/// convert integral n to string, write to dest
//  return the Iter pointing after the last position of writing
template <typename T, typename Iter = char*>
Iter chuan( Iter dest, T n, typename enable_if<std::is_integral<T>>::type* = 0 )
{
  static const char digit_pairs[201] = {
      "00010203040506070809"
      "10111213141516171819"
      "20212223242526272829"
      "30313233343536373839"
      "40414243444546474849"
      "50515253545556575859"
      "60616263646566676869"
      "70717273747576777879"
      "80818283848586878889"
      "90919293949596979899"
  };

  int sign = -(n < 0);
  if (sign)
    *dest++ = '-';
  typename std::make_unsigned<T>::type val = (n ^ sign) - sign;
  size_t size = digits(val);
  Iter iter = dest + (size - 1);

  while (val >= 100) {
    size_t pos = (val % 100) * 2;
    val /= 100;
    *iter-- = digit_pairs[pos + 1];
    *iter-- = digit_pairs[pos];
  }
  while (val > 0) {
    *iter-- = '0' + (val % 10);
    val /= 10;
  }

  return dest + size;
}

/// specialized version for string literals, save the call to strlen
//
template <typename T, size_t n, typename Iter = char*>
inline Iter chuan( Iter dest, const T(&str)[n] )
{
  std::copy(str + 0, str + n, dest);
  return dest + n;
}

template <typename T, typename Iter = char*>
inline Iter chuan( Iter dest, const T* str )
{
  size_t size = strlen(str);
  std::copy(str, str + size, dest);
  return dest + size;
}

} } // namespace ku::chuan

