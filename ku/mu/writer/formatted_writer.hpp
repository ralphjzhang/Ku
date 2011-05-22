#ifndef KU_MU_FORMATTED_WRITER_HPP
#define KU_MU_FORMATTED_WRITER_HPP

#include "writer/basic_writer.hpp"


namespace ku { namespace mu {


class formatted_writer : public basic_writer
{
public:
    typedef formatted_record record_type;

    formatted_writer() = default;
    ~formatted_writer() = default;

    void write(record_type& r)
    {
        r.apply_format();
        std::for_each(sink_list_.begin(), sink_list_.end(), [&r](sink_ptr& sp) {
                if (sp->pass(r.get_level()))
                {
                    sp->consume(r.get().str());
                    sp->flush();
                }
            });

        r.clear();
    }

};


}}


#endif // KU_MU_FORMATTED_WRITER_HPP
