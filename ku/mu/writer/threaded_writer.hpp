#ifndef KU_MU_THREADED_WRITER_HPP
#define KU_MU_THREADED_WRITER_HPP

#define _GLIBCXX_HAS_GTHREADS 1


#include <queue>
#include <mutex>
/*#include <thread>
#include <atomic>
#include <condition_variable>

#undef _GLIBCXX_HAS_GTHREADS
#undef _GLIBCXX__PTHREADS
#undef _POSIX_TIMEOUTS
*/
/*
namespace ku { namespace mu {


template <typename Base>
class threaded_writer : public Base
{
public:
    typedef Base base_type;
    typedef threaded_writer<base_type> this_type;
    typedef typename base_type::record_type record_type;

    threaded_writer()
    {
        exit_flag_ = false;
        writer_thread_ = std::thread(&this_type::do_write, this);
    }

    ~threaded_writer()
    {
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            exit_flag_ = true;
            queue_cond_.notify_one();
        }

        if (writer_thread_.joinable())
            writer_thread_.join();
    }

    void write(record_type& r)
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        record_queue_.push(r);
        queue_cond_.notify_one();
    }

private:
    void do_write()
    {
        while(!exit_flag_)
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            queue_cond_.wait(lock,
                    [&record_queue_, &exit_flag_]{return !record_queue_.empty() && exit_flag_;}
                );
            while (!record_queue_.empty())
            {
                base_type::write(record_queue_.front());
                record_queue_.pop();
            }
        }
    }

private:
    typedef std::queue<record_type> queue_type;
    mutable std::mutex queue_mutex_;
    std::condition_variable queue_cond_;
    queue_type record_queue_;

    std::atomic<bool> exit_flag_;
    std::thread writer_thread_;
};


}}

*/
#endif // KU_MU_THREADED_WRITER_HPP
