#include "level.hpp"
#include "sink/console.hpp"
#include "sink/simple_file.hpp"
#include "record/formatted_record.hpp"
#include "reader/formatted_reader.hpp"
#include "writer/basic_writer.hpp"
#include "writer/formatted_writer.hpp"
#include "writer/threaded_writer.hpp"
#include "logger.hpp"
#include "macro.hpp"
#include "mu.hpp"

/*
namespace ku { namespace mu {

namespace threaded {

typedef ku::mu::formatted_reader                            reader_type;
typedef ku::mu::threaded_writer<ku::mu::formatted_writer>   writer_type;
typedef logger<reader_type, writer_type>                    logger_type;

inline auto logger(ku::mu::level::type l = level::none) -> logger_type&
{
    return ku::mu::get_logger<reader_type, writer_type>(l);
}

}

}}

#define MU_THREADED_LOG(l)  ku::mu::threaded::logger(l).set_context(MU_FUNC, MU_FILE_LINE);\
                            ku::mu::threaded::logger(l)

void init_threaded_writer_log()
{
    using namespace ku::mu;
    using namespace ku::mu::field;
    using namespace ku::mu::level;

    sink_ptr console_sink_ptr = make_sink<sink::console>();
    console_sink_ptr->set_filter([](level::type l){
            return l >= level::trace;
        });

    sink_ptr simple_file_sink_ptr = make_sink<sink::simple_file>("mu.log");
    simple_file_sink_ptr->set_filter(level::info);

    threaded::logger().set_format(
            lit('['), process_id, lit(':'), thread_id, lit(' '), date_time, lit(' '), field::level, lit("]: "),
            message,
            lit(" ("), func, lit(' '), file_line, lit(")\n")
        );
    MU_THREADED_LOG(all).adopt_sink(std::move(console_sink_ptr));
    MU_THREADED_LOG(all).adopt_sink(std::move(simple_file_sink_ptr));
}

int threaded_writer_test1()
{
    using namespace ku::mu::level;

    MU_THREADED_LOG(info) << "From writer thread" << eor;
    return 0;
}
*/
