/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once
#include <cstddef>
#include <vector>

namespace ku { namespace fusion {

class Sequence;

class ProcessorBarrier
{
public:
  ProcessorBarrier() = default;
  ~ProcessorBarrier() = default;

  size_t max_sequence() const;
  size_t min_sequence() const;

  template <typename Processor>
  void add_processor(Processor const& pr)
  {
    processor_seqs_.push_back(&(pr.sequence()));
  }

private:
  std::vector<Sequence const*> processor_seqs_; 
};

} } // namespace ku::fusion

