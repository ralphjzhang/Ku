#ifndef KU_MU_COMMON_HPP
#define KU_MU_COMMON_HPP

#include <sys/types.h>

#include "string_type.hpp"


namespace ku { namespace mu {

namespace field
{


struct level_field
{
    typedef string_type value_type;
};


struct file_line_field
{
    typedef char const* value_type;
};


struct func_field
{
    typedef char const* value_type;
};


struct lit_field
{
    typedef string_type value_type;

    lit_field operator()(char c)
    {
        value.assign(1, c);
        return *this;
    }

    lit_field operator()(string_type&& s)
    {
        value.assign(s);
        return *this;
    }

    string_type value;

};


struct message_field
{
    typedef string_type value_type;
};


struct date_time_field
{
    typedef string_type value_type;

    date_time_field(const char* format = "%b %d %H:%M:%S")
            : format(format)
    {
    }

    string_type format;
};


struct process_id_field
{
    typedef pid_t value_type;
};

struct thread_id_field
{
    typedef unsigned value_type;
};


namespace {

level_field         level;
file_line_field     file_line;
func_field          func;
lit_field           lit;
message_field       message;
date_time_field     date_time;
process_id_field    process_id;
thread_id_field     thread_id;

}


}

}}

#endif // KU_MU_COMMON_HPP
