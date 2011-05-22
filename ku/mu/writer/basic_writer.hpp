#ifndef KU_MU_BASIC_WRITER_HPP
#define KU_MU_BASIC_WRITER_HPP

#include <algorithm>

#include "sink.hpp"
#include "record.hpp"


namespace ku { namespace mu {


class basic_writer
{
public:
    typedef record record_type;
    typedef std::vector<sink_ptr> sink_list;

    basic_writer() = default;
    basic_writer(basic_writer const&) = delete;
    ~basic_writer() = default;

    void adopt_sink(sink_ptr&& p)
    {
        sink_list_.push_back(std::forward<sink_ptr&&>(p));
    }

    void write(record_type& r)
    {
        std::for_each(sink_list_.begin(), sink_list_.end(), [&r](sink_ptr& sp) {
                if (sp->pass(r.get_level()))
                {
                    sp->consume(r.get().str());
                    sp->flush();
                }
            });

        r.clear();
    }

protected:
    sink_list sink_list_;
};


}}


#endif // KU_MU_BASIC_WRITER_HPP
