#include <iostream>
#include <type_traits>
#include "variadic.hpp"
#include "func_traits.hpp"

int main()
{
  using namespace ku::yuan;
  using namespace std;

  static_assert(is_same<int, head<int, double>::type>::value, "typelist_::head");
  static_assert(3 == length<int, double, int>::value, "typelist_::length");
  static_assert(is_same<int, at<0, int, double>::type>::value, "typelist_::at");
  static_assert(is_same<double, at<1, int, double>::type>::value, "typelist_::at");
}

