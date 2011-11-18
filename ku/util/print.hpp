#pragma once
#include <iostream>

namespace ku { namespace util {

/// type-safe printf
//
void printf(const char* s)
{
  while (*s) {
    if (*s == '%' && *++s != '%')
      throw std::runtime_error("format string missing arguments");
    std::cout << *s++;
  }
}
namespace aux {
template <typename T>
bool check_specifier(const char*& p) 
{
  switch (*p) {
  case 'd':
  case 'i':
    return is_same<T, int>::value;
  case 'e':
  case 'E':
    return is_same<T, double>::value;
  case 's':
    return is_same<T, char*>::value;
  case 'Z':
    return true; // user-defined
  //... other specifiers TBD
  }
  return false;
}
} // namespace aux

template <typename T, typename... Args>
void printf(const char*s, const T& value, const Arg&... args)
{
  while (*s) {
    if (*s == '%' && *++s != '%') {
      if (aux::check_specifier<T>(*s)) {
        std::cout << value;
      } else {
        throw std::runtime_error("invalid printf specifier");
      }
      return printf(++s, args...);
    }
    std::cout << *s++;
  }
  throw std::runtime_error("extra arguments to printf");
}

} } // namespace ku::util

