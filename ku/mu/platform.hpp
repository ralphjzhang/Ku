#ifndef KU_MU_PLATFORM_HPP
#define KU_MU_PLATFORM_HPP

#ifdef WIN32
//#define _WIN32_WINNT 0x0601
#include <Windows.h>
#include <pthread.h>
#endif


namespace std
{

/*
template<typename CharT, class Traits>
inline basic_ostream<CharT, Traits>&
operator<<(basic_ostream<CharT, Traits>& out, pthread_t tid)
{
    return out << ::GetThreadId(pthread_getw32threadhandle_np(tid));
}
*/
}


namespace ku { namespace mu {


inline unsigned current_thread_id()
{
#ifdef WIN32
    return GetCurrentThreadId();
#endif
}


}}


#endif // KU_MU_PLATFORM_HPP
