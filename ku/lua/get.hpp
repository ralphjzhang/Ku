#pragma once
#include <string>
#include <lua.hpp>

namespace ku { namespace lua {

std::string get_string(lua_State* L, char const* expr);

double get_number(lua_State* L, char const* expr);

bool get_bool(lua_State* L, char const* expr);

template <typename T>
T get(lua_State* L, char const* expr);

template <>
inline std::string get(lua_State* L, char const* expr) { return get_string(L, expr); }

template <>
inline double get(lua_State* L, char const* expr) { return get_number(L, expr); }

template <>
inline bool get(lua_State* L, char const* expr) { return get_bool(L, expr); }

template <>
inline long get(lua_State* L, char const* expr) { return static_cast<long>(get_number(L, expr)); }

template <>
inline int get(lua_State* L, char const* expr) { return static_cast<int>(get_number(L, expr)); }

/*
template <typename T>
auto get(lua_State* L, char const* expr)
  -> typename std::enable_if<std::is_integral<T>::value, T>::type
{ return static_cast<T>(get_number(L, expr)); }
*/

} } // namespace ku::lua

