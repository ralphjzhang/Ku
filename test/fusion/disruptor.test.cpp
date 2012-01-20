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
  EXPECT_EQ(CAP - 1, buffer.cursor());
  Sequence seq1(buffer.cursor()); // TODO this is a bit tricky: gating sequences should start from 1 slot before buffer capacity to make wrapping correct, think of it
  buffer.set_gatings({&seq1});
  EXPECT_TRUE(buffer.has_available(1));
  EXPECT_TRUE(buffer.has_available(512));
  EXPECT_TRUE(buffer.has_available(1024));
}

TEST(RingBuffer, claim_next)
{
  size_t const CAP = 1024u;
  RingBuffer<Entry, YieldWaiting> buffer(1000);
  Sequence seq1(buffer.cursor()); // TODO this is a bit tricky: gating sequences should start from 1 slot before buffer capacity to make wrapping correct, think of it
  buffer.set_gatings({&seq1});

  size_t claimed_seq = buffer.claim_next();
  EXPECT_EQ(CAP, claimed_seq); // claimed sequence starts from buffer capacity
  EXPECT_TRUE(buffer.has_available(1));
  EXPECT_TRUE(buffer.has_available(512));
  EXPECT_TRUE(buffer.has_available(CAP - 1));
  EXPECT_FALSE(buffer.has_available(CAP));
  buffer[claimed_seq].data = 42;
  buffer.publish(claimed_seq);
  EXPECT_EQ(CAP, buffer.cursor()); // after 1 slot published, cursor increase from (CAP - 1) to CAP
  EXPECT_EQ(42, buffer.get(claimed_seq).data);

  claimed_seq = buffer.claim_next(10);
  EXPECT_EQ(CAP + 10, claimed_seq);
  EXPECT_TRUE(buffer.has_available(1));
  EXPECT_TRUE(buffer.has_available(512));
  EXPECT_TRUE(buffer.has_available(CAP - 11));
  EXPECT_FALSE(buffer.has_available(CAP - 10));
  EXPECT_FALSE(buffer.has_available(CAP));
  buffer.publish(claimed_seq - 7); // suppose we only publish 3 out 10 claimed slots
  EXPECT_EQ(CAP + 3, buffer.cursor()); // then cursor should move further 3 slots
}

