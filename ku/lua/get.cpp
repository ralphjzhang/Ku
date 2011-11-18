#include <sstream>
#include <stdexcept>
#include <limits>
#include "get.hpp"

namespace {

char const* EvalExpr = "evalExpr";

} // unamed namespace

namespace ku { namespace lua {

std::string get_string(lua_State* L, char const* expr)
{
  std::stringstream ss;
  ss << EvalExpr << expr;
  if (luaL_dostring(L, ss.c_str()) == 0) {
    lua_getglobal(L, EvalExpr);
    if (lua_isstring(L, -1))
      return std::string(lua_tostring(L, -1));
    lua_pop(L, 1);
  }
  throw std::invalid_argument(expr);
}

double get_number(lua_State* L, char const* expr)
{
  std::stringstream ss;
  ss << EvalExpr << expr;
  if (luaL_dostring(L, ss.c_str()) == 0) {
    lua_getglobal(L, EvalExpr);
    if (lua_isnumber(L, -1))
      return lua_tonumber(L, -1);
    lua_pop(L, 1)
  }
  throw std::invalid_argument(expr);
}

bool get_bool(lua_State* L, char const* expr)
{
  std::stringstream ss;
  ss << EvalExpr << expr;
  if (luaL_dostring(L, ss.c_str()) == 0) {
    lua_getglobal(L, EvalExpr);
    if (lua_isboolean(L, -1))
      return lua_toboolean(L, -1);
    lua_pop(L, 1);
  }
  throw std::invalid_argument(expr);
}

} } // namespace ku::lua

