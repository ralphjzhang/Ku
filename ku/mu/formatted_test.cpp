#include "level.hpp"
#include "sink/console.hpp"
#include "sink/simple_file.hpp"
#include "reader/formatted_reader.hpp"
#include "writer/formatted_writer.hpp"
#include "macro.hpp"
#include "mu.hpp"


void init_formatted_log()
{
    using namespace ku::mu;
    using namespace ku::mu::level;
    using namespace ku::mu::field;

    sink_ptr console_sink_ptr = make_sink<sink::console>();
    console_sink_ptr->set_filter([](level::type l){
            return l >= level::trace;
        });

    sink_ptr simple_file_sink_ptr = make_sink<sink::simple_file>("mu.log");
    simple_file_sink_ptr->set_filter(level::info);

    formatted::logger().set_format(
            lit('['), process_id, lit(':'), thread_id, lit(' '), date_time, lit(' '), field::level, lit("]: "),
            message,
            lit(" ("), func, lit(' '), file_line, lit(")\n")
        );
    MU_FORMAT_LOG(all).adopt_sink(std::move(console_sink_ptr));
    MU_FORMAT_LOG(all).adopt_sink(std::move(simple_file_sink_ptr));

}


void formatted_test1()
{
    using namespace ku::mu::level;

    init_formatted_log();

    MU_FORMAT_LOG(info) << "Hello " << 42 << " world!" << eor;
    MU_FORMAT_LOG(warning) << "This is a warning message." << eor;

}
