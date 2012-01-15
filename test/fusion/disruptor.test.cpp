#include <utest.hpp>
#include <ku/fusion/disruptor/sequence.hpp>
#include <ku/fusion/disruptor/ring_buffer.hpp>
#include <ku/fusion/disruptor/claimer.hpp>
#include <ku/fusion/disruptor/waiting.hpp>


using namespace ku::fusion::disruptor;

TEST(Sequence, sequence)
{

}

struct Entry
{
  int data;
};

TEST(RingBuffer, ctor)
{
  size_t const CAP = 1024u;
  RingBuffer<Entry, YieldWaiting> buffer(1000);

  EXPECT_EQ(CAP, buffer.capacity());
  Sequence seq1(CAP - 1); // TODO this is a bit tricky: gating sequences should start from 1 slot before buffer capacity to make wrapping correct, think of it
  buffer.set_gatings({&seq1});
  EXPECT_TRUE(buffer.has_available(1));
  EXPECT_TRUE(buffer.has_available(512));
  EXPECT_TRUE(buffer.has_available(1024));
}

TEST(RingBuffer, claim_next)
{
  size_t const CAP = 1024u;
  RingBuffer<Entry, YieldWaiting> buffer(1000);
  Sequence seq1(CAP - 1); // TODO this is a bit tricky: gating sequences should start from 1 slot before buffer capacity to make wrapping correct, think of it
  buffer.set_gatings({&seq1});

  EXPECT_EQ(CAP, buffer.claim_next()); // claimed sequence starts from buffer capacity
  EXPECT_TRUE(buffer.has_available(1));
  EXPECT_TRUE(buffer.has_available(512));
  EXPECT_TRUE(buffer.has_available(CAP - 1));
  EXPECT_FALSE(buffer.has_available(CAP));
}

