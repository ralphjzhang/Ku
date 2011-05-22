#ifndef KU_SU_NONCOPYABLE_HPP
#define KU_SU_NONCOPYABLE_HPP


namespace ku { namespace su {

namespace unspecified {

struct noncopyable
{
    noncopyable() = default;
    noncopyable(noncopyable const&) = delete;
    noncopyable& operator=(noncopyable const&) = delete;
};

}

using namespace unspecified;

}}


#endif // KU_SU_NONCOPYABLE_HPP
