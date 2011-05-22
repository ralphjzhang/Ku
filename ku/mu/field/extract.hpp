#ifndef KU_MU_FIELD_EXTRACT_HPP
#define KU_MU_FIELD_EXTRACT_HPP

#include <ctime>
#include <cstring>
#include <unistd.h>
#include <pthread.h>

#include "level.hpp"
#include "platform.hpp"
#include "field/common.hpp"


namespace ku { namespace mu {

namespace field {


template<typename Record, typename Field>
typename Field::value_type
extract(Record const&, Field const&);


template<typename Record>
level_field::value_type
extract(Record const& r, level_field const&)
{
    return level::to_str(r.get_level());
}

template<typename Record>
message_field::value_type
extract(Record const& r, message_field const&)
{
    return r.get_message();
}

template<typename Record>
file_line_field::value_type
extract(Record const& r, file_line_field const&)
{
    char const* begin = r.get_file_line();
    char const* end = begin + strlen(begin);
    while(end != begin && *--end != '\\')
        ;
    return end+1;
}

template<typename Record>
func_field::value_type
extract(Record const& r, func_field const&)
{
    return r.get_func();
}

template<typename Record>
lit_field::value_type
extract(Record const& r, lit_field const& l)
{
    return l.value;
}

template<typename Record>
date_time_field::value_type
extract(Record const& r, date_time_field const& dt)
{
    // Unsafe, but should be fine.
    char value[40];
    time_t rawtime;
    tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(value, 40, dt.format.c_str(), timeinfo);

    return value;
}

template<typename Record>
process_id_field::value_type
extract(Record const&, process_id_field const&)
{
    return getpid();
}

template<typename Record>
thread_id_field::value_type
extract(Record const&, thread_id_field const&)
{
    return current_thread_id();
}

}

}}


#endif // KU_MU_FIELD_EXTRACT_HPP
