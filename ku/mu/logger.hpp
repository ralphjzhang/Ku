#ifndef KU_MU_LOGGER_HPP
#define KU_MU_LOGGER_HPP


namespace ku { namespace mu {


struct eor_type
{};


template <typename Reader, typename Writer>
class logger : public Reader, public Writer
{
public:
    typedef Reader reader_type;
    typedef Writer writer_type;
    typedef logger<reader_type, writer_type> this_type;
    typedef typename reader_type::record_type record_type;

    logger() = default;
    ~logger() = default;
    logger(logger const&) = delete;
    logger& operator=(logger const&) = delete;

    template <typename T>
    this_type& operator<<(T const& t)
    {
        reader_type::read(t);
        return *this;
    }

    this_type& operator<<(eor_type&)
    {
        writer_type::write(reader_type::get_record());
        return *this;
    }
};


class basic_writer;

template<typename Reader, typename Writer = basic_writer>
auto get_logger(level::type l = level::none) -> logger<Reader, Writer>&
{
    static logger<Reader, Writer> lg;
    lg.get_record().set_level(l);
    return lg;
}



}}


#endif // KU_MU_LOGGER_HPP
