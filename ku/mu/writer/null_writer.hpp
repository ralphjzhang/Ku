#ifndef KU_MU_NULL_WRITER_HPP
#define KU_MU_NULL_WRITER_HPP

#include "sink.hpp"
#include "record.hpp"


namespace ku { namespace mu {


class null_writer
{
public:
    typedef record record_type;

    null_writer() = default;
    null_writer(null_writer const&) = delete;
    ~null_writer() = default;

    void adopt_sink(sink_ptr&& p)
    {
    }

    void write(record_type& r)
    {
    }

};


}}


#endif // KU_MU_NULL_WRITER_HPP
