#pragma once
#include <utility>
#include <vector>
#include <lua.hpp>
#include <ku/reflex/func_traits.hpp>
#include "wrap_func.hpp"

namespace ku { namespace lua {

class Yue
{
public:
  template <typename... Args>
  Yue(lua_State *L, char const* libname, Args... fp)
      : libname_(libname)
      , funcs_({Args::call...})
      , names_({ku::reflex::func_traits(Args::function()).name()...})
  {
    register_funcs(L);
  }

  ~Yue() = default;
  
private:
  void register_funcs(lua_State *L);

  std::string libname_;
  std::vector<lua_CFunction> funcs_; 
  std::vector<std::string>   names_;
};

} } // namespace ku::lua

