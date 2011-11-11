/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once

namespace ku { namespace fusion { namespace util {

// if_handle_inbound
template <typename T, typename... Args>
auto if_handle_inbound(T& t, Args&... args)
  -> decltype(std::declval<T>().handle_inbound(args...))
{ return t.handle_inbound(args...); }

template <typename... T> void if_handle_inbound(T&...)
{ }

// if_handle_outbound
template <typename T, typename... Args>
auto if_handle_outbound(T& t, Args&... args)
  -> decltype(std::declval<T>().handle_outbound(args...))
{ return t.handle_outbound(args...); }

template <typename... T> void if_handle_outbound(T&...)
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
{ return false; } // Default: remove handler on error

} } } // namespace ku::fusion::util


