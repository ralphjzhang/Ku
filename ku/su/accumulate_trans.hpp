#pragma once

namespace ku { namespace su {

template <typename T, typename Trans, typename Acc, typename... Ranges>
T accumulate_trans(T init, Trans trans, Acc acc, Ranges const&... r)
{
}

} } // namespace ku::su

