#pragma once
#include <execinfo.h>
#include <vector>
#include <string>
#include <algorithm>
#include <ku/reflex/type_traits.hpp>

namespace ku { namespace reflex {

namespace aux {

std::string demangle_backtrace(char* symbol)
{
  char *mangled_start = nullptr, *mangled_end = nullptr;
  for (char* p = symbol; *p; ++p) {
    if (*p == '(')
      mangled_start = p;
    else if (*p == '+')
      mangled_end = p;
    else if (*p == ')')
      break;
  }

  if (mangled_start && mangled_end) {
    ++mangled_start;      // Move after '('
    *mangled_end = '\0';  // Prepare the c string
    std::string ret(symbol, mangled_start);
    ret.append(demangle(mangled_start));
    *mangled_end = '+';   // Recover the symbol
    ret.append(mangled_end);
    return ret;
  } else {
    return symbol;
  }
}

} // namespace aux

std::vector<std::string> backtrace( bool demangle_symbols = true )
{
  // A running system seldom exceeds 64 level of stacks, even if it exceeds, the upper 64 stacks
  // should provide enough information for diagnostic
  constexpr size_t capacity = 64;
  void *stack[capacity];

  size_t size = ::backtrace(stack, capacity);
  char ** symbols = backtrace_symbols (stack, size);

  std::vector<std::string> ret;
  // Skip the first entry (backtrace( ) itself)
  for (size_t i = 1; i < size && symbols; ++i)
    ret.emplace_back(demangle_symbols ? aux::demangle_backtrace(symbols[i]) : symbols[i]);

  ::free(symbols);
  return ret;
}

// Handler, register it with std::signal
void backtrace_handler(int sig)
{
  std::vector<std::string> bt(backtrace(true));
  // The uppermost 2 stacks are the handler, and backtrace itself
  if (bt.size() > 2)
    std::for_each(bt.begin() + 2, bt.end(), [](std::string const& s) {
        std::cerr << s << std::endl;
      });
  exit(1);
}

} } // namespace ku::reflex

