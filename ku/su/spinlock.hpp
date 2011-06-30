#pragma once

namespace ku { namespace su {

class spinlock
{
public:
  spinlock( ) = default;
  spinlock( spinlock const& ) = default;
  ~spinlock( ) = default;

  bool try_lock( ) {
    int r = __sync_lock_test_and_set(&flag_, 1);
    return r == 0;
  }

  void lock( ) {
    while (!try_lock())
      while (flag_)
        ;
  }

  void unlock( ) {
    __sync_lock_release(&flag_);
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
  int flag_;
};

} } // namespace ku::su

