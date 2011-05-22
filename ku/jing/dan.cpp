#include <iostream>
#include <ku/dan/dan.hpp>
#include "type_traits.hpp"
#include "func_traits.hpp"

struct s0 {};

void f0() {}
double f1(int) {}
int f2(int, double) {}

void f10(int) {}
int f10(double) {}

template <typename T> void tf0() {}

int main()
{
  using namespace std;
  using namespace ku::jing;
  using namespace ku::dan;

  expect(type_traits(42).name() ==  "int");
  expect(type_traits(s0()).name() == "s0");
  expect(func_traits(f0).arity() == 0);
  expect(func_traits(f0).name() == "f0");
  expect(func_traits(f0).fullname() == "void f0()");
  expect(func_traits(tf0<int>).name() == "tf0<int>");
  expect(func_traits(tf0<int>).fullname() == "void tf0<int>()");
  expect(func_traits(f1).result() == "double");
  expect(func_traits(f1).is_template() == false);
  expect(func_traits(tf0<int>).is_template() == true);
  expect(func_traits(f1).arg<1>().name() == "int");
  expect(func_traits(f2).arg<2>().name() == "double");
  //expect(func_traits<int(double)>(f10).name());
};

