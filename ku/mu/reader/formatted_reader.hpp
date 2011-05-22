#ifndef KU_MU_FORMATTED_READER_HPP
#define KU_MU_FORMATTED_READER_HPP

#include "record/formatted_record.hpp"


namespace ku { namespace mu {


class formatted_reader
{
public:
    typedef formatted_record record_type;

    formatted_reader() = default;
    ~formatted_reader() = default;

    auto get_record() -> record_type&
    {
        return record_;
    }

    template<typename T>
    auto read(T const& t) -> void
    {
        record_.append(t);
    }

    auto set_context(char const* func = nullptr, char const* file_line = nullptr) -> void
    {
        record_.set_context(func, file_line);
    }

    template<typename ...Args>
    void set_format(Args const&... args)
    {
        record_.set_format(args...);
    }

public:
    record_type record_;
};


}}

#endif // KU_MU_FORMATTED_READER_HPP
