#include <iostream>
#include <type_traits>
#include "variadic.hpp"
#include "func_traits.hpp"
#include "type_traits.hpp"

void f0() {}
double f1(int) {}
int f2(int, double) {}
template <typename T> void tf0() {}

struct s0 {};
template <typename T> struct t0 {};

int main()
{
  using namespace ku::yuan;
  using namespace std;

  // variadic.hpp
  static_assert(is_same<int, head<int, double>::type>::value, "head<>::type");
  static_assert(3 == length<int, double, int>::value, "length<>::value");
  static_assert(is_same<int, at<0, int, double>::type>::value, "at<>::type");
  static_assert(is_same<double, at<1, int, double>::type>::value, "at<>::type");
  static_assert(0 == ku::yuan::find<int, int, double>::value, "find<>::value");
  static_assert(-1 == ku::yuan::find<int, double>::value, "find<>::value");

  // func_traits.hpp
  static_assert(0 == func_traits<decltype(f0)>::arity, "func_traits<>::arity");
  static_assert(1 == func_traits<decltype(f1)>::arity, "func_traits<>::arity");
  static_assert(is_same<double, func_traits<decltype(f1)>::result_type>::value, "func_traits<>::result_type");
  static_assert(is_same<int, func_traits<decltype(f1)>::arg<1>::type>::value, "func_traits<>::arg<>::type");

  // type_traits.hpp
  static_assert(0 == is_template<s0>::value, "is_template<>::value");
  static_assert(1 == is_template<t0<int>>::value, "is_template<>::value");
  //static_assert(0 == is_template<decltype(f0)>::value, "is_template<>::value");
}

