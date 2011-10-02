#pragma once
#include <system_error>
#include <ku/util/noncopyable.hpp>

struct addrinfo;
struct sockaddr_in;

namespace ku { namespace net {
  
class address;
 
namespace socket {

class handle : private ku::util::noncopyable
{
public:
  explicit handle(int raw_handle)
    : raw_handle_(raw_handle)
  { }

  template <typename Err>
  handle(int raw_handle, Err err)
    : raw_handle_(raw_handle)
  { set_error(err); }

  handle(handle&& h)
    : raw_handle_(h.raw_handle_), error_(h.error_)
  { h.clear(); }

  int raw_handle() const { return raw_handle_; }

  std::error_code error() const { return error_; }
  void set_error(int err_no) { set_error(static_cast<std::errc>(err_no)); }
  void set_error(std::errc err) { error_ = std::make_error_code(err); }
  void set_error(std::error_code const& ec) { error_ = ec; }

  void clear() { raw_handle_ = 0; error_.clear(); }

private:
  int raw_handle_;
  std::error_code error_;
};

handle create(addrinfo const& addr);

handle& bind_listen(handle& h, address const& addr);

//channel::handle accept(handle& h, address& addr);
ssize_t read(handle const& h, void* buf, size_t count);
ssize_t write(handle const& h, void* buf, size_t count);

handle& close(handle& h);

} } } // namespace ku::net::socket

