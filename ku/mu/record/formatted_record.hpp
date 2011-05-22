#ifndef KU_MU_FORMATTED_RECORD_HPP
#define KU_MU_FORMATTED_RECORD_HPP

#include <memory>
#include <su/tuple.hpp>

#include "string_type.hpp"
#include "record.hpp"
#include "field/extract.hpp"


// Remove after nullptr support added
#define nullptr NULL

namespace ku { namespace mu {


class formatted_record : public record
{
public:
    class format
    {
    public:
        format(formatted_record& rec)
                : record_(rec)
        {}

        virtual void apply() = 0;

    protected:
        formatted_record& record_;
    };

    typedef std::shared_ptr<format> format_ptr;

public:
    formatted_record() = default;
    ~formatted_record() = default;

    formatted_record(formatted_record const& rhs)
            : record(rhs)
    {
        formatted_message_ << rhs.formatted_message_.str();
        format_ptr_ = rhs.format_ptr_;
    }

    template<typename ...Args>
    void set_format(Args const&... args);

    auto set_context(char const* func = nullptr, char const* file_line = nullptr) -> void
    {
        file_line_ = file_line;
        func_ = func;
    }

    void clear()
    {
        formatted_message_.str("");
        record::clear();
    }

    auto get_file_line() const -> char const*
    {
        return file_line_;
    }

    auto get_func()const -> char const*
    {
        return func_;
    }

    auto get_message() const -> string_type
    {
        return message_.str();
    }

    stream_type& get()
    {
        return formatted_message_;
    }

    void apply_format()
    {
        if (format_ptr_)
            format_ptr_->apply();
    }


    struct extractor
    {
        extractor(formatted_record& r)
                : record(r)
        {}

        template <typename T>
        void operator()(T const& t) const
        {
            record.formatted_message_ << extract(record, t);
        }

        formatted_record& record;
    };

protected:
    char const* file_line_;
    char const* func_;

    stream_type formatted_message_;
    format_ptr format_ptr_;
};


template <typename ...Args>
class basic_format : public formatted_record::format
{
    typedef std::tuple<Args...> fields_type;

public:
    basic_format() = delete;
    ~basic_format() = default;
    basic_format(basic_format const&) = default;
    basic_format& operator=(basic_format const&) = default;

    basic_format(formatted_record& rec, Args const&... args)
            : format(rec)
            , fields_(std::make_tuple(args...))
    {
    }

    fields_type const& get_fields() const
    {
        return fields_;
    }

    virtual void apply()
    {
        using ku::su::tuple_for_each;
        tuple_for_each(fields_, formatted_record::extractor(record_));
    }

private:
    fields_type fields_;
};


typedef formatted_record::format_ptr format_ptr;


template<typename ...Args>
void formatted_record::set_format(Args const&... args)
{
    format_ptr_ = std::make_shared<basic_format<Args...>>(*this, args...);
}


}}


#endif // KU_MU_FORMATTED_RECORD_HPP
