#ifndef KU_MU_SINK_HPP
#define KU_MU_SINK_HPP

#include <memory>
#include <functional>

#include "string_type.hpp"
#include "level.hpp"

namespace ku { namespace mu {


class sink_base
{
public:
    typedef std::function<bool(level::type)> filter_type;

    sink_base()
    {
        set_filter(level::none);
    }

    virtual ~sink_base() {}
    sink_base(sink_base const&) = delete;
    sink_base& operator=(sink_base const&) = delete;

public:
    void set_filter(filter_type const& f)
    {
        filter_ = f;
    }

    void set_filter(level::type l)
    {
        filter_ = [l](level::type record_level){
                return record_level >= l;
            };
    }

    bool pass(level::type record_level) const
    {
        return filter_(record_level);
    }

    virtual void consume(string_type const&) = 0;
    virtual void flush() {}

private:
    filter_type filter_;

};

typedef std::unique_ptr<sink_base> sink_ptr;

template<typename Sink, typename ...Args>
sink_ptr make_sink(Args... args)
{
    return sink_ptr(new Sink(args...));
}


}}

#endif // KU_MU_SINK_HPP
