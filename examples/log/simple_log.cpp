#include <ku/log/logger.hpp>
#include <ku/log/collector.hpp>
#include <ku/log/console_sink.hpp>
#include <iostream>

int main()
{
  using namespace ku::log;
  logger();

  logger().writer().add_sink(Sink_ptr(new ConsoleSink()));
  collector(LogLevel::Debug).self() << "Hello11111";
  collector(LogLevel::Debug).self() << "World22222";
  collector(LogLevel::Debug).self() << "Ralph33333";
  collector(LogLevel::Debug).self() << "ABCDE44444";
  collector(LogLevel::Debug).self() << "FGHIJ55555";
  collector(LogLevel::Debug).self() << "KLMNO66666";
  collector(LogLevel::Debug).self() << "PQRST77777";
  collector(LogLevel::Debug).self() << "UVWXY88888";
}

