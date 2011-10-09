#pragma once

namespace ku { namespace net { namespace buffer {

/**
 * Buffer is a concept, any type T that satisfies
 *
 * size_t ku::net::buffer::size(T const& t);
 * void* ku::net::buffer::raw_begin(T& t);
 *
 * models a buffer.
 * Operations for common buffer types are provided, user can extend to any type as needed.
 **/

template <typename Container>
inline size_t size(Container const& c) { return c.size(); }

template <typename Container>
inline void* raw_begin(Container& c) { return &*c.begin(); }

// Specialization for char arrays
template <size_t N>
inline size_t size(const char (&buf)[N]) { return N; }

inline void* raw_begin(char *buf) { return buf; }

} } } // namespace ku::net::buffer

