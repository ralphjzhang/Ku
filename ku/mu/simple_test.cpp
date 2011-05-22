#include "sink/console.hpp"
#include "writer/basic_writer.hpp"
#include "reader/basic_reader.hpp"
#include "mu.hpp"


void init_simple_log()
{
  using namespace ku::mu;
  using namespace ku::mu::level;

  sink_ptr console_sink_ptr = make_sink<sink::console>();
  console_sink_ptr->set_filter(level::info);
  basic::logger(all).adopt_sink(std::move(console_sink_ptr));
}


void simple_test1()
{
  init_simple_log();

  using namespace ku::mu;
  using namespace ku::mu::level;

  std::cout << "Starting simple_test1() --- " << std::endl;

  basic::logger(info) << "Test\n" << eor;
  basic::logger(warning) << "Warning Test\n" << eor;
  basic::logger(debug) << "This line should not appear" << eor;
}

