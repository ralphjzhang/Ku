#pragma once
#include <array>
#include <cstring>

namespace ku { namespace log {

class Collector
{
public:
  Collector()
    : end_(buffer_.begin()), count_(1)
  { buffer_[0] = '\n'; }

  void append(std::string const& s)
  {
    count_ += s.size();
    if (count_ >= 1024)
      ; // TODO resize
    end_ = strcpy(end_, s.c_str());
    *end_ = '\n';
  }

  template <typename Int>
  void append(Int i)
  {
    // TODO
  }

private:
  std::array<char, 1024> buffer_;
  char *end_;
  size_t count_;
};

template <typename T>
Collector& operator << (Collector& c, T const& t)
{
  c.append(t);
  return c;
}

template <typename T>
Collector& operator << (Collector& c, T&& t)
{
  // TODO optimize for rvalue?
  return c;
}

template <typename T>
Collector& operator , (Collector& c, T const& t)
{
  // TODO: printf style
  return c;
}

} } // namespace ku::log

