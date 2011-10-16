/*
 * Copyright 2011, Zhang, Jun. All rights reserved.
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com)
 *
 * This source code is provided with absolutely no warranty.
 */ 
#pragma once
#include <system_error>
#include "util.hpp"

namespace ku { namespace net {

/**
 * Handle is a thin wrapper to OS file descriptor (raw_handle) with an error_code.
 * A Handle can own the file descriptor or not, the ownership can be retrieved by
 * owner(). Copying is prohibited to prevent multiple owners.
 * 
 * A file descriptor can have at most one owner, the owner is responsible for the 
 * life cycle (closing the file descriptor when destroyed). It is possible that a 
 * file descriptor has no owner at all, in which case the user is responsible for 
 * closing it.
 *
 * The ownership can be transferred with C++ move semantics. The handle being moved
 * is cleared after the move. The ownership can also be released by calling method
 * release_handle(), in both cases, the releasing handle still references the file
 * descriptor, but don't manage its life cycle.
 *
 * Handle is a wrapper. It's not supposed to be used polymorphically. It has no
 * virtual methods at all (not even destructor).
 **/
class Handle : private util::noncopyable
{
  friend class Channel;

public:
  explicit Handle(int raw_handle, bool owner) 
    : owner_(owner), raw_handle_(raw_handle)
  { }

  template <typename Err>
  Handle(int raw_handle, Err err, bool owner)
    : owner_(owner), raw_handle_(raw_handle)
  { set_error(err); }

  Handle(Handle&& h)
    : owner_(h.owner_), raw_handle_(h.raw_handle_), error_(h.error_)
  { h.clear(); }

  ~Handle() { if (owner()) close(); }

  bool owner() const { return owner_; }
  int raw_handle() const { return raw_handle_; }
  int release_handle() { int ret = raw_handle_; clear(); return ret; }

  std::error_code error() const { return error_; }
  void set_error(int err_no) { set_error(static_cast<std::errc>(err_no)); }
  void set_error(std::errc err) { error_ = std::make_error_code(err); }
  void set_error(std::error_code const& ec) { error_ = ec; }
  
  void close();

private:
  Handle() : owner_(false), raw_handle_(0) { }
  void clear() { owner_ = false; raw_handle_ = 0; error_.clear(); }
  // adopt ownership from another Handle, this is only for Channel to use.
  void adopt(Handle&& h);

  bool owner_;
  int raw_handle_;
  std::error_code error_;
};

} } // namespace ku::net

