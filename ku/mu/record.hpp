#ifndef KU_MU_RECORD_HPP
#define KU_MU_RECORD_HPP

#include <memory>
#include <sstream>

#include "level.hpp"


namespace ku { namespace mu {


class record
{
public:
    typedef std::stringstream stream_type;

    record() = default;
    ~record() = default;

    record(record const& rhs)
    {
        level_ = rhs.level_;
        message_ << rhs.message_.str();
    }

    void clear()
    {
        message_.str("");
    }

    void set_level(level::type l)
    {
        level_ = l;
    }

    level::type get_level() const
    {
        return level_;
    }

    stream_type& get()
    {
        return message_;
    }

    template<typename T>
    void append(T const& t)
    {
        message_ << t;
    }

protected:
    level::type level_;
    stream_type message_;
};


typedef std::shared_ptr<record> record_ptr;


}}

#endif // KU_MU_RECORD_HPP
