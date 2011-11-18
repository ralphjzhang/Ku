#include <pthread.h>
#pragma once

namespace ku { namespace util {

class Spinlock
{
public:
  Spinlock() { pthread_spin_init(&lock_, 0); }
  ~Spinlock() { pthread_spin_destroy(&lock_); }

  inline bool try_lock() { return pthread_spin_trylock(&lock_); }
  inline void lock() { pthread_spin_lock(&lock_); }
  inline void unlock( ) { pthread_spin_unlock(&lock_); }

private:
  pthread_spinlock_t lock_;
};

} } // namespace ku::util

