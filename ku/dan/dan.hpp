#pragma once
#include <gtest/gtest.h>

/// dan: Unit test framework, currently just wrap around google test
//
namespace ku { namespace dan {

inline void init(int* argc, char** argv)
{
  testing::InitGoogleTest(argc, argv);
}

} } // namespace ku::dan

