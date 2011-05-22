#ifndef KU_MU_SINK_NULL_HPP
#define KU_MU_SINK_NULL_HPP

#include <string>


namespace ku { namespace mu {

namespace sink {


class null_sink: public sink_base
{
public:
    null_sink(std::string const& file_name)
    {
    }

    ~null_sink() {}

    void consume(string_type const& s)
    {
    }

    void flush()
    {
    }

};


}

}}

#endif // KU_MU_SINK_NULL_HPP
