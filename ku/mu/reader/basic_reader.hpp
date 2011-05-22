#ifndef KU_MU_BASIC_READER_HPP
#define KU_MU_BASIC_READER_HPP

#include "record.hpp"


namespace ku { namespace mu {


class basic_reader
{
public:
    typedef record record_type;

    basic_reader() = default;
    ~basic_reader() = default;

    auto get_record() -> record_type&
    {
        return record_;
    }

    template<typename T>
    auto read(T const& t) -> void
    {
        record_.append(t);
    }

private:
    record_type record_;
};


}}


#endif // KU_MU_BASIC_READER_HPP
