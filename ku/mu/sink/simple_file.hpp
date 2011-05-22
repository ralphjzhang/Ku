#ifndef KU_MU_SINK_SIMPLE_FILE_HPP
#define KU_MU_SINK_SIMPLE_FILE_HPP

#include <fstream>

#include "sink.hpp"

namespace ku { namespace mu {

namespace sink {


class simple_file : public sink_base
{
public:
    simple_file(std::string const& file_name)
            : fs_(file_name)
    {
    }

    virtual ~simple_file() {}

    virtual void consume(string_type const& s)
    {
        fs_ << s;
    }

    virtual void flush()
    {
        fs_.flush();
    }

private:
    std::fstream fs_;
};


}

}}


#endif // KU_MU_SINK_SIMPLE_FILE_HPP
