#ifndef KU_MU_HPP
#define KU_MU_HPP

#include "reader/basic_reader.hpp"
#include "reader/formatted_reader.hpp"
#include "writer/basic_writer.hpp"
#include "writer/formatted_writer.hpp"
#include "logger.hpp"


#define MU_FORMAT_LOG(l)    ku::mu::formatted::logger(l).set_context(MU_FUNC, MU_FILE_LINE);\
                            ku::mu::formatted::logger(l)


namespace ku { namespace mu {


namespace {

eor_type eor;

}


namespace basic {

typedef ku::mu::basic_reader                        reader_type;
typedef ku::mu::basic_writer                        writer_type;
typedef ku::mu::logger<reader_type, writer_type>    logger_type;

inline auto logger(ku::mu::level::type l = level::none) -> logger_type&
{
    return ku::mu::get_logger<reader_type, writer_type>(l);
}


}


namespace formatted {

typedef ku::mu::formatted_reader                    reader_type;
typedef ku::mu::formatted_writer                    writer_type;
typedef ku::mu::logger<reader_type, writer_type>    logger_type;

inline auto logger(ku::mu::level::type l = level::none) -> logger_type&
{
    return ku::mu::get_logger<reader_type, writer_type>(l);
}


}


}}

using ku::mu::eor;

#endif // KU_MU_HPP
