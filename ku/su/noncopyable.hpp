#pragma once

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

} } // namespace ku::su


