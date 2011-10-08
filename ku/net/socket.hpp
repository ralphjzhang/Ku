#pragma once
#include <system_error>
#include <ku/util/noncopyable.hpp>

struct addrinfo;
struct sockaddr_in;

namespace ku { namespace net {
  
class Address;
 
class Socket : private ku::util::noncopyable
{
private:
  explicit Socket(int raw_handle)
    : raw_handle_(raw_handle)
  { }

  template <typename Err>
  Socket(int raw_handle, Err err)
    : raw_handle_(raw_handle)
  { set_error(err); }

public:
  static Socket create(addrinfo const& addr);

  Socket(Socket&& h)
    : raw_handle_(h.raw_handle_), error_(h.error_)
  { h.clear(); }

  int raw_handle() const { return raw_handle_; }

  Socket& bind_listen(Address const& addr);
  Socket accept(Address& addr) const;

  std::error_code error() const { return error_; }
  void set_error(int err_no) { set_error(static_cast<std::errc>(err_no)); }
  void set_error(std::errc err) { error_ = std::make_error_code(err); }
  void set_error(std::error_code const& ec) { error_ = ec; }

  void clear() { raw_handle_ = 0; error_.clear(); }
  void close();

private:
  int raw_handle_;
  std::error_code error_;
};


ssize_t read(Socket const& h, void* buf, size_t count);
ssize_t write(Socket const& h, void* buf, size_t count);


} } // namespace ku::net

