#include <atomic>
#pragma once

namespace ku { namespace util {

class spinlock
{
public:
  spinlock() = default;
  ~spinlock() = default;

  bool try_lock( ) {
    flag_ = true;
    return flag_;
  }

  void lock( ) {
    while (!try_lock())
      ;
  }

  void unlock( ) {
    while (flag_)
      flag_ = false;
  }

public:
  class scoped_lock
  {
  private:
    spinlock & sp_;
    scoped_lock( scoped_lock const& ) = delete;
    scoped_lock & operator = ( scoped_lock const& ) = delete;

  public:
    explicit scoped_lock( spinlock & sp ) : sp_(sp) {
      sp_.lock();
    }

    ~scoped_lock( ) {
      sp_.unlock();
    }
  };

private:
  std::atomic_bool flag_;
};

} } // namespace ku::util

