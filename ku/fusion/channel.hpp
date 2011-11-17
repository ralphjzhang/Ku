/***************************************************************
 * Copyright 2011, Zhang, Jun. All rights reserved.            *
 * Author: Zhang, Jun (ralph dot j dot zhang at gmail dot com) *
 *                                                             *
 * This source code is provided with absolutely no warranty.   *
 ***************************************************************/ 
#pragma once

namespace ku { namespace fusion {

class Channel
{
public:
  enum Type { Pub, Sub, Req, Rep };

  void bind(char const* endpoint);
  void send();
  void recv();

private:
};

} } // namespace ku::fusion

