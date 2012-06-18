#pragma once
#include <type_traits>
#include <memory>

namespace ku { namespace util {

template<typename T, typename... Args>
inline auto construct(Args&&... args) -> typename std::remove_const<T>::type*
{
  using T_nc = typename std::remove_const<T>::type;
  return new T_nc(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
inline auto make_unique(Args&&... args) -> std::unique_ptr<T>
{
  using T_nc = typename std::remove_const<T>::type;
  return std::unique_ptr<T_nc>(construct<T_nc>(args...));
}

} } // namespace ku::util

