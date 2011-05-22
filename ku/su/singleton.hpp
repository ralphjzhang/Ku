#ifndef KU_SU_SINGLETON_HPP
#define KU_SU_SINGLETON_HPP

#include <mutex>
#include "noncopyable.hpp"

namespace ku { namespace su {


using std::call_once;
using std::once_flag;

template<typename DerivedT, typename StorageT = DerivedT>
class lazy_singleton
        : private noncopyable
{
public:
    typedef DerivedT derived_type;
    typedef StorageT storage_type;

public:
    static auto get() -> storage_type&
    {
        static once_flag flag;
        call_once(flag, &DerivedT::init_instance);

        return get_instance();
    }

    static auto init_instance() -> void
    {
        get_instance();
    }

protected:
    static auto get_instance() -> storage_type&
    {
        static storage_type instance;
        return instance;
    }
};


template<typename DerivedT, typename StorageT = DerivedT>
class singleton
        : public lazy_singleton<DerivedT, StorageT>
{
public:
    typedef StorageT storage_type;
    static storage_type& instance;
};

template<typename DerivedT, typename StorageT>
StorageT& singleton<DerivedT, StorageT>::instance =
        lazy_singleton<DerivedT, StorageT>::get();


template<typename SingletonT>
SingletonT& get_singleton()
{
    return singleton<SingletonT>::get();
}

}}


#endif // KU_SU_SINGLETON_HPP

