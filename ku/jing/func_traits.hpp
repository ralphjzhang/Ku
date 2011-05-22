#pragma once
#include <string>
#include <utility>
#include <ku/yuan/func_traits.hpp>
#include <ku/jing/type_traits.hpp>

namespace ku { namespace jing {

namespace aux {

template <typename R, typename... Args>
struct func_traits
{
private:
  typedef R(function_type)(Args...);
  function_type* fp;

  std::string _fullname( )
  {
    Dl_info info;
    if (dladdr(reinterpret_cast<const void*>(fp), &info))
      return std::string(ku::jing::demangle(info.dli_sname));
    return std::string();
  }
  
  inline bool _is_template(std::string const& name)
  {
    return '>' == name[name.find('(') - 1];
  }
public:
  func_traits(function_type f) : fp(f)
  {}

  std::string fullname()
  {
    std::string name(_fullname());
    // For non-template function, need to prepend result_type name
    if (!_is_template(name)) 
      return result().append(" ").append(name);
    else
      return name;
  }

  std::string name()
  {
    std::string name(_fullname());
    name.erase(name.find('('));
    // Remove result_type from template function name
    size_t begin = _is_template(name) ? result().size() : 0;
    return std::string(name, begin);
  }

  inline size_t arity()
  { 
    return sizeof...(Args);
  }

  inline std::string result()
  {
    return type_traits<R>().name();
  }

  inline bool is_template()
  {
    return _is_template(_fullname());
  }

  template <size_t N>
  auto arg() -> aux::type_traits<typename ku::yuan::at<N - 1, Args...>::type>
  {
    return aux::type_traits<typename ku::yuan::at<N - 1, Args...>::type>();
  }
};

} // namespace aux

template <typename R, typename... Args>
auto func_traits( R(&fp)(Args...) = *reinterpret_cast<R(*)(Args...)>(0) ) -> aux::func_traits<R, Args...>
{
  return aux::func_traits<R, Args...>(fp);
}

} } // namespace ku::jing

