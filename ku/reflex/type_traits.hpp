#pragma once
#include <string>
#include <cxxabi.h>
#include <dlfcn.h>

namespace ku { namespace reflex {

std::string demangle( char const* name )
{
  int status;
  char* demangled = abi::__cxa_demangle(name, 0, 0, &status);
  std::string ret(demangled ? demangled : "");
  ::free(demangled);
  return ret;
}

namespace aux {

template <typename T>
struct type_traits
{
  std::string name( )
  {
    return demangle(typeid(T).name());
  }
};

} // namespace aux

template <typename T>
auto type_traits( T const& = *reinterpret_cast<T*>(0) ) -> aux::type_traits<T>
{
  return aux::type_traits<T>();
}

auto type_traits( ) -> aux::type_traits<void>
{
  return aux::type_traits<void>();
}

} } // namespace ku::reflex

