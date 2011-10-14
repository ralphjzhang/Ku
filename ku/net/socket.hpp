#pragma once
#include <system_error>
#include "util.hpp"

struct addrinfo;

namespace ku { namespace net {
  
class Address;
 
class Socket : private util::noncopyable
{
private:
  explicit Socket(int raw_handle) : raw_handle_(raw_handle) { }

  template <typename Err>
  Socket(int raw_handle, Err err) : raw_handle_(raw_handle) { set_error(err); }

public:
  Socket(Socket&& s);
  ~Socket() { close(); }

  static Socket create(addrinfo const& addr);
  int raw_handle() const { return raw_handle_; }
  int release_handle();

  Socket& listen(Address const& addr);
  Socket accept(Address& addr) const;
  bool listening() const { return listening_; }

  std::error_code error() const { return error_; }
  void set_error(int err_no) { set_error(static_cast<std::errc>(err_no)); }
  void set_error(std::errc err) { error_ = std::make_error_code(err); }
  void set_error(std::error_code const& ec) { error_ = ec; }

  void clear() { raw_handle_ = 0; error_.clear(); }
  void close();

private:
  int raw_handle_;
  std::error_code error_;
  bool listening_;
};

template <typename Buffer>
ssize_t write(Socket const& h, Buffer& buf)
{
  //return ::write(h.raw_handle(), buf, count);
  return 0;
}


} } // namespace ku::net

