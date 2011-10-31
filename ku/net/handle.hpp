#pragma once
#include <system_error>
#include <unistd.h>
#include "util.hpp"

namespace ku { namespace net {

namespace ops {
struct Common;
} // namespace ku::net::ops

// This is a hack to help template parameter be friend, shouldn't need this after gcc4.7
template <typename T>
struct FriendMaker
{
  typedef T Type;
};

// =======================================================================================
// Handle is a thin wrapper to OS file descriptor (raw_handle) with an error_code.
// A Handle can own the file descriptor or not, the ownership can be retrieved by
// owner(). Copying is prohibited to prevent multiple owners.
// 
// A file descriptor can have at most one owner, the owner is responsible for the 
// life cycle (closing the file descriptor when destroyed). It is possible that a 
// file descriptor has no owner at all, in which case the user is responsible for 
// closing it.
//
// The ownership can be transferred with C++ move semantics. The handle being moved
// is cleared after the move. The ownership can also be released by calling method
// release_handle().
//
// Handle is a wrapper. It's not supposed to be used polymorphically. It has no
// virtual methods at all (not even destructor).
// =======================================================================================
template <typename T>
class Handle
{
  friend class ops::Common;
  friend class FriendMaker<T>::Type;
  friend class NoticeBoard;
  friend class WeakHandle;

public:
  Handle(Handle&& h) : owner_(h.owner_), raw_handle_(h.raw_handle_), error_(h.error_)
  { h.clear(); }
  ~Handle() { if (owner()) close(); }

  bool owner() const { return owner_; }
  bool valid() const { return raw_handle_ > 0; }
  explicit operator bool () { return valid(); }

  std::error_code const& error() const { return error_; }
  void clear_error() { error_.clear(); }
  bool close();

private:
  // Handle() : owner_(false), raw_handle_(0) { } // TODO made public?
  explicit Handle(int raw_handle, bool owner = true)
    : owner_(owner), raw_handle_(raw_handle)
  { }

  int raw_handle() const { return raw_handle_; }
  void clear() { owner_ = false; raw_handle_ = 0; error_.clear(); }
  void set_error(int err_no) { set_error(static_cast<std::errc>(err_no)); }
  void set_error(std::errc err) { error_ = std::make_error_code(err); }

  bool owner_;
  int raw_handle_;
  std::error_code error_;
};

template <typename T>
bool Handle<T>::close()
{
  if (!valid())
    return true;
  set_error(::close(raw_handle_) == -1 ? errno : 0);
  if (!error())
    raw_handle_ = 0;
  return !error();
}


// =======================================================================================
// WeakHandle doens't own the file descriptor. It's nothing more than a temporary storage
// =======================================================================================
class WeakHandle
{
  friend class NoticeBoard;

public:
  WeakHandle() = delete;
  ~WeakHandle() = default;
  template <typename T>
  WeakHandle(Handle<T> const& h) : raw_handle_(h.raw_handle_) { }

private:
  int raw_handle() const { return raw_handle_; }
  int raw_handle_;
};

} } // namespace ku::net

