#pragma once

namespace ku { namespace net { namespace buffer {

/**
 * Buffer is a concept, any type T that satisfies
 *
 * size_t ku::net::buffer::size(T const& t);
 * void* ku::net::buffer::raw_begin(T& t);
 *
 * is a buffer.
 * Operations for common buffer types are provided, user can extend to any type as needed.
 **/

template <typename Container>
inline size_t size(Container const& c) { return c.size(); }

template <typename Container>
void* raw_begin(Container& c) { return &*c.begin(); }

} } } // namespace ku::net::buffer

