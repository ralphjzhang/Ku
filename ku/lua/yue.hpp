#pragma once
#include <utility>
#include <vector>
#include <lua.hpp>
#include <ku/reflex/func_traits.hpp>
#include "wrap_func.hpp"

namespace ku { namespace yue {

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
  void register_funcs(lua_State *L)
  {
    size_t size = funcs_.size();
    luaL_reg *map = new luaL_reg[size + 1];
    map[size] = {NULL, NULL};
    for (int i = 0; i < size; ++i) {
      map[i] = {names_[i].c_str(), funcs_[i]};
    }
    luaL_register(L, libname_.c_str(), map);
    delete[] map;
  }

  std::string libname_;
  std::vector<lua_CFunction> funcs_; 
  std::vector<std::string>   names_;
};

} } // namespace ku::yue

