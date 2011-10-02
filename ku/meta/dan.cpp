#include <iostream>
#include <type_traits>
#include <ku/dan/dan.hpp>
#include "variadic.hpp"
#include "func_traits.hpp"
#include "type_traits.hpp"

using namespace ku::meta;
using namespace std;

void f0() {}
double f1(int) { return 0.; }
int f2(int, double) { return 1; }
template <typename T> void tf0() {}

struct s0 {};
template <typename T> struct t0 {};

TEST(meta, variadic)
{
  static_assert(is_same<int, head<int, double>::type>::value, "head<>::type");
  static_assert(3 == length<int, double, int>::value, "length<>::value");
  static_assert(is_same<int, at<0, int, double>::type>::value, "at<>::type");
  static_assert(is_same<double, at<1, int, double>::type>::value, "at<>::type");
  static_assert(0 == ku::meta::find<int, int, double>::value, "find<>::value");
  static_assert(-1 == ku::meta::find<int, double>::value, "find<>::value");
}

TEST(meta, func_traits)
{
  static_assert(0 == func_traits<decltype(f0)>::arity, "func_traits<>::arity");
  static_assert(1 == func_traits<decltype(f1)>::arity, "func_traits<>::arity");
  static_assert(is_same<double, func_traits<decltype(f1)>::result_type>::value, "func_traits<>::result_type");
  static_assert(is_same<int, func_traits<decltype(f1)>::arg<1>::type>::value, "func_traits<>::arg<>::type");
}

TEST(meta, type_traits)
{
  static_assert(0 == is_template<s0>::value, "is_template<>::value");
  static_assert(1 == is_template<t0<int>>::value, "is_template<>::value");
  //static_assert(0 == is_template<decltype(f0)>::value, "is_template<>::value");
}

