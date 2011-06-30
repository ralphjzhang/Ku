#include <string>
#include <iostream>
#include <type_traits>

#include "memory.hpp"
#include "sequence_number.hpp"

int main()
{
  using namespace ku::su;

  auto sp = construct<std::string>(1, 'e');
  auto sptr = make_unique<std::string>(1, 'e');

  //bachelor::get();

  for (int i = 0; i < 10; ++i)
    std::cout << next_seq() << std::endl;
}
