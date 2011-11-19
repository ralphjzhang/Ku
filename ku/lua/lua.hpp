#pragma once
#include <lua.hpp>
#include "get.hpp"

namespace ku { namespace lua {

class Lua
{
public:
  Lua() : lua_(luaL_newstate()) { }
  ~Lua() { if (lua_) lua_close(lua_); }

  bool load(char const* s) { return luaL_dostring(lua_, s) == 0; }

  template <typename T>
  T get(char const* expr) { return ku::lua::get<T>(lua_, expr); } 

private:
  lua_State* lua_;
};

} } // namespace ku::lua

