#pragma once
#include <vector>
#include <array>

namespace ku { namespace net {

/**
 * raw_buffer provides a means that any data type interact with read/write
 *
 * void* ku::net::buffer::raw_buffer(T& t);
 *
 * Operations for common buffer types are provided, user can extend to any type as needed.
 **/

inline void* raw_buffer(void *buf) { return buf; }

template <typename T>
inline void* raw_buffer(std::vector<T>& buf) { return &*buf.begin(); }

template <typename T, size_t N>
inline void* raw_buffer(std::array<T, N>& buf) { return &*buf.begin(); }


} } // namespace ku::net

