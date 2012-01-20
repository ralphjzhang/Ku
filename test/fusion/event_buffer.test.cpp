#include <utest.hpp>
#include <thread>
#include <chrono>
#include <ku/fusion/sequence.hpp>
#include <ku/fusion/event_buffer.hpp>

using namespace ku::fusion;

TEST(EventBuffer, ctor)
{
  EventBuffer eb1(1);
  EXPECT_EQ(1u, eb1.capacity());

  EventBuffer eb2(3);
  EXPECT_EQ(4u, eb2.capacity());

  EventBuffer eb(1000);
  EXPECT_EQ(1024u, eb.capacity());
  EXPECT_EQ(eb.initial_sequence(), eb.claimed_sequence());
  EXPECT_EQ(eb.initial_sequence(), eb.published_sequence());
}

struct Processor
{
  Processor(size_t init) : seq(init) {}
  Sequence seq;
  Sequence const& sequence() const { return seq; }
};

TEST(EventBuffer, claim_publish)
{
  EventBuffer eb(1000);
  Processor p1(eb.initial_sequence());
  eb.barrier().add_processor(p1);

  // claim, publish
  size_t claim_seq = eb.initial_sequence() + 1;
  eb.claim(claim_seq);
  EXPECT_EQ(claim_seq, eb.claimed_sequence());
  EXPECT_EQ(eb.initial_sequence(), eb.published_sequence());
  eb.publish(claim_seq);
  EXPECT_EQ(claim_seq, eb.published_sequence());
  EXPECT_EQ(claim_seq, eb.claimed_sequence());

  // claim, claim, publish
  claim_seq += 100;
  eb.claim(claim_seq);
  EXPECT_EQ(claim_seq, eb.claimed_sequence());
  claim_seq += 100;
  eb.claim(claim_seq);
  EXPECT_EQ(claim_seq, eb.claimed_sequence());
  eb.publish(claim_seq - 100);
  EXPECT_EQ(claim_seq - 100, eb.published_sequence());
  EXPECT_EQ(claim_seq, eb.claimed_sequence());
}

TEST(EventBuffer, claim_wait)
{
  using namespace std::chrono;

  bool waited = false;

  EventBuffer eb(1000);
  Processor p1(eb.initial_sequence());
  eb.barrier().add_processor(p1);
  eb.set_wait_strategy([&](size_t count){
    waited = true;
    std::this_thread::sleep_for(milliseconds(1));
  });

  size_t claim_seq = eb.initial_sequence() + 10;
  eb.claim(claim_seq);
  eb.publish(claim_seq);
  std::thread publisher([&]{
    // now we have 1014 free slots, so claiming 1020 would trigger a wait
    eb.claim(claim_seq + 1020);
  });
  std::this_thread::sleep_for(microseconds(500));
  EXPECT_TRUE(waited);
  p1.seq.set(claim_seq); // processor catches up, wait finishes 
  publisher.join();
}

TEST(EventBuffer, max_usable)
{
  EventBuffer eb(1000);
  Processor p1(eb.initial_sequence());
  eb.barrier().add_processor(p1);

  // initial state
  size_t end_seq = eb.initial_sequence() + eb.capacity();
  EXPECT_EQ(end_seq, eb.max_usable_sequence());
  EXPECT_EQ(end_seq, eb.max_usable_block_sequence());

  // claim, publish (usable sequence shouldn't change)
  size_t claim_seq = eb.initial_sequence() + 1;
  eb.claim(claim_seq);
  eb.publish(claim_seq);
  EXPECT_EQ(end_seq, eb.max_usable_sequence());
  EXPECT_EQ(end_seq, eb.max_usable_block_sequence());

  // processor catches up 1 slot
  p1.seq.set(claim_seq);
  EXPECT_EQ(end_seq + 1, eb.max_usable_sequence());
  EXPECT_EQ(end_seq, eb.max_usable_block_sequence());

  // claim, publish (usable sequence shouldn't change)
  claim_seq += 10;
  eb.claim(claim_seq);
  eb.publish(claim_seq);
  EXPECT_EQ(end_seq + 1, eb.max_usable_sequence());
  EXPECT_EQ(end_seq, eb.max_usable_block_sequence());

  // processor catches up 4 more slots
  p1.seq.set(p1.seq.get() + 4);
  EXPECT_EQ(end_seq + 5, eb.max_usable_sequence());
  EXPECT_EQ(end_seq, eb.max_usable_block_sequence());
  // processor catches all 11 published slots
  p1.seq.set(claim_seq);
  end_seq += 11;
  EXPECT_EQ(end_seq, eb.max_usable_sequence());

  // claim/publish wraps around the ring, notice this should happen in poller,
  // coz the storage is not continous
  claim_seq += 1020;
  eb.claim(claim_seq);
  eb.publish(claim_seq);
  EXPECT_EQ(end_seq, eb.max_usable_sequence());
  EXPECT_EQ(end_seq, eb.max_usable_block_sequence());

  // processor catches up 1 more slot
  p1.seq.set(p1.seq.get() + 1);
  end_seq += 1;
  EXPECT_EQ(end_seq, eb.max_usable_sequence());
  EXPECT_EQ(end_seq, eb.max_usable_block_sequence());

  // processor catches to the end of continous block
  p1.seq.set(p1.seq.get() + 1012);
  end_seq += 1012;
  EXPECT_EQ(end_seq, eb.max_usable_sequence());
  EXPECT_EQ(end_seq, eb.max_usable_block_sequence());

  // processor catches over the ring "gap"
  p1.seq.set(p1.seq.get() + 1);
  end_seq += 1;
  EXPECT_EQ(end_seq, eb.max_usable_sequence());
  EXPECT_EQ(end_seq - 1, eb.max_usable_block_sequence());
  p1.seq.set(p1.seq.get() + 1);
  EXPECT_EQ(end_seq + 1, eb.max_usable_sequence());
  EXPECT_EQ(end_seq - 1, eb.max_usable_block_sequence());
}

