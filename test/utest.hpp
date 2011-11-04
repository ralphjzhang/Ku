#pragma once
#include <gtest/gtest.h>

/// utest: Unit test framework, currently just wrap around google test
//
namespace ku { namespace utest {

inline void init(int* argc, char** argv)
{
  testing::InitGoogleTest(argc, argv);
}

} } // namespace ku::utest

