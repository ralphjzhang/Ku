#ifndef KU_SU_MEMORY_HPP
#define KU_SU_MEMORY_HPP

#include <type_traits>
#include <memory>


namespace ku { namespace su {


template<typename T, typename... Args>
inline auto construct(Args&&... args) -> typename std::remove_const<T>::type*
{
    typedef typename std::remove_const<T>::type T_nc;
    return new T_nc(std::forward<Args>(args)...);
}


template<typename T, typename... Args>
inline auto make_unique(Args&&... args) -> std::unique_ptr<T>
{
    typedef typename std::remove_const<T>::type T_nc;
    return std::unique_ptr<T_nc>(construct<T_nc>(args...));
}



}}



#endif // KU_SU_MEMORY_HPP
