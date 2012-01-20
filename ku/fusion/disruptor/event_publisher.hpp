/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once

namespace ku { namespace fusion { namespace disruptor {

template <typename RingBuffer>
class EventPublisher
{
public:
  typedef typename RingBuffer::EventType EventType;

  EventPublisher(RingBuffer& buffer)
    : buffer_(buffer)
  {}
  ~EventPublisher() = default;

  void publish(EventType const& event);

private:
  RingBuffer& buffer_;
};


template <typename RingBuffer>
void EventPublisher::publish(EventType const& event)
{
  size_t seq = buffer_.claim_next();
  buffer_[seq] = event; // TODO this can be more generic with a translator
  // A properly implemented RingBuffer guarantees that publish(seq) is not re-ordered before
  // event is written to the slot, so "publish" has the "commit" semantic
  buffer_.publish(seq);
}


} } } // namespace ku::fusion::disruptor

