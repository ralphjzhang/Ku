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
digits( T t, typename enable_if<std::is_unsigned<T>>::type* = nullptr )
{
  return
    (t >= 10000u)
    ? (t >= 10000000u) 
      ? (t >= 1000000000u) ? 10 : (t >= 100000000u) ? 9 : 8
      : (t >= 1000000u) ? 7  : (t >= 100000u) ? 6 : 5
    : (t >= 100u)
      ? (t >= 1000u) ? 4 : 3
      : (t >= 10u)  ? 2 : 1;
}

/// find number of digits (power of 10) in an UNSIGNED integral number
//  for 64-bit
template <typename T>
typename enable_if_c<(sizeof(T) > 4), int>::type
digits( T t, typename enable_if<std::is_unsigned<T>>::type* = nullptr )
{
  return (t <= 0xFFFFFFFF) ? digits(uint32_t(t)) :
    (t >= 100000000000000u)
    ? (t >= 100000000000000000u)
      ? (t >= 10000000000000000000u) ? 20 : (t >= 1000000000000000000u) ? 19 : 18
      : (t >= 10000000000000000u) ? 17 : (t >= 1000000000000000u) ? 16 : 15
    : (t >= 1000000000000u)
      ? (t >= 10000000000000u) ? 14 : 13
      : (t >= 100000000000u) ? 12 : (t >= 10000000000u) ? 11 : 10;
}

/// convert integral n to string, write to dest
//  return the Iter pointing after the last position of writing
template <typename T, typename Iter = char*>
Iter chuan( Iter dest, T n, typename enable_if<std::is_integral<T>>::type* = nullptr )
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

template <typename T, typename Iter = char*>
Iter chuan( Iter dest, T n, typename enable_if<std::is_floating_point<T>>::type* = nullptr )
{
  // TODO temp solution
  return dest + sprintf(dest, "%f", n);
}

/// specialized version for string literals, save the call to strlen
//
template <typename T, size_t n, typename Iter = char*>
inline Iter chuan( Iter dest, const T(&str)[n] )
{
  return std::copy(str + 0, str + n - 1, dest);
}

/// specialized version for plain c strings
//
template <typename T, typename Iter = char*>
inline Iter chuan( Iter dest, const T* str )
{
  return std::copy(str, str + strlen(str), dest);
}

/// specialized version for c++ standard strings
//
template <typename Iter = char*>
inline Iter chuan( Iter dest, std::string const& str)
{
  return std::copy(str.begin(), str.end(), dest);
}


} } // namespace ku::chuan

