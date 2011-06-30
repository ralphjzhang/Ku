#pragma once
#include <string>

namespace ku { namespace mu {

  namespace level
  {

    enum class type: unsigned short
    {
      none = 0x00,
      trace = 0x01,
      debug = 0x02,
      info = 0x04,
      warning = 0x08,
      error = 0x10,
      fatal = 0x20,

      all = 0xff
    };


    inline auto to_str(type l) -> std::string
    {
      switch (l)
      {
      case level::none:
        return "none";
        break;
      case level::trace:
        return "trace";
        break;
      case level::debug:
        return "debug";
        break;
      case level::info:
        return "info";
        break;
      case level::warning:
        return "warning";
        break;
      case level::error:
        return "error";
        break;
      case level::fatal:
        return "fatal";
        break;
      case level::all:
        return "all";
        break;
      default:
        return "unknown";
        break;
      }
    }


    inline auto from_str(std::string const& s) -> level::type
    {
      if (s == "none")
        return level::none;
      if (s == "trace")
        return level::trace;
      if (s == "debug")
        return level::debug;
      if (s == "info")
        return level::info;
      if (s == "warning")
        return level::warning;
      if (s == "error")
        return level::error;
      if (s == "fatal")
        return level::fatal;
      if (s == "all")
        return level::all;

      return level::none;
    }

  }


} } // namespace ku::mu

