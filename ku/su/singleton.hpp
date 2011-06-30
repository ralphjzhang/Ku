#pragma once
#include <mutex>
#include <ku/su/noncopyable.hpp>

namespace ku { namespace su {

using std::call_once;
using std::once_flag;

template<typename DerivedT, typename StorageT = DerivedT>
class lazy_singleton : private noncopyable
{
public:
  typedef DerivedT derived_type;
  typedef StorageT storage_type;

public:
  static storage_type& get()
  {
    static once_flag flag;
    call_once(flag, &DerivedT::init_instance);
    return get_instance();
  }

  static void init_instance()
  {
    get_instance();
  }

protected:
  static storage_type& get_instance()
  {
    static storage_type instance;
    return instance;
  }
};


template<typename DerivedT, typename StorageT = DerivedT>
class singleton : public lazy_singleton<DerivedT, StorageT>
{
public:
  typedef StorageT storage_type;
  static storage_type& instance;
};

template<typename DerivedT, typename StorageT>
StorageT& singleton<DerivedT, StorageT>::instance = lazy_singleton<DerivedT, StorageT>::get();


template<typename SingletonT>
SingletonT& get_singleton()
{
  return singleton<SingletonT>::get();
}

} } // namespace ku::su


