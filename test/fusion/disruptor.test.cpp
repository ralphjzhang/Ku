#include <utest.hpp>
#include <ku/fusion/disruptor/sequence.hpp>
#include <ku/fusion/disruptor/ring_buffer.hpp>
#include <ku/fusion/disruptor/claimer.hpp>
#include <ku/fusion/disruptor/waiting.hpp>


using namespace ku::fusion::disruptor;

TEST(Sequence, sequence)
{

}

TEST(RingBuffer, ctor)
{
  struct Entry {
  };

  RingBuffer<Entry, SinglePublisherClaimer, BlockingWaiting> buffer(1000);
  EXPECT_EQ(1024u, buffer.capacity());
}

