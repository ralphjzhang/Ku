#pragma once

namespace ku { namespace net { namespace util {

// if_handle_accept
template <typename T, typename... Args>
auto if_handle_accept(T& t, Args&... args)
  -> decltype(std::declval<T>().handle_accept(args...))
{ return t.handle_accept(args...); }

template <typename... T> bool if_handle_accept(T&...)
{ return true; }

// if_handle_timer_tick
template <typename T, typename... Args>
auto if_handle_timer_tick(T& t, Args&... args)
  -> decltype(std::declval<T>().handle_timer_tick(args...))
{ return t.handle_timer_tick(args...); }

template <typename... T> void if_handle_timer_tick(T&...)
{ }

// if_handle_read
template <typename T, typename... Args>
auto if_handle_read(T& t, Args&... args)
  -> decltype(std::declval<T>().handle_read(args...))
{ return t.handle_read(args...); }

template <typename... T> void if_handle_read(T&...)
{ }

// if_handle_write
template <typename T, typename... Args>
auto if_handle_write(T& t, Args&... args)
  -> decltype(std::declval<T>().handle_write(args...))
{ return t.handle_write(args...); }

template <typename... T> void if_handle_write(T&...)
{ }

// if_handle_close
template <typename T, typename... Args>
auto if_handle_close(T& t, Args&... args)
  -> decltype(std::declval<T>().handle_close(args...))
{ return t.handle_close(args...); }

template <typename... T> void if_handle_close(T&...)
{ }

// if_handle_error
template <typename T, typename... Args>
auto if_handle_error(T& t, Args&... args)
  -> decltype(std::declval<T>().handle_error(args...))
{ return t.handle_error(args...); }

template <typename... T> bool if_handle_error(T&...)
{ return false; }

} } } // namespace ku::net::util

