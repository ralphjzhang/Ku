#ifndef KU_MU_SINK_CONSOLE_HPP
#define KU_MU_SINK_CONSOLE_HPP

#include <iostream>

#include "sink.hpp"


namespace ku { namespace mu {

namespace sink {


class console : public sink_base
{
public:
    console() = default;
    virtual ~console() {}

    virtual void consume(string_type const& s)
    {
        std::cerr << s;
    }

    virtual void flush()
    {
        std::cerr.flush();
    }

};


}

}}

#endif //KU_MU_SINK_CONSOLE_HPP
