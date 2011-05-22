#include <iostream>
#include <ku/dan/dan.hpp>
#include "chuan.hpp"

int main()
{
  using namespace ku::dan;
  using namespace ku::chuan;
  char buf[100] = "";
  const char *s = "world!!";

  *chuan(buf, "hello") = '\0';
  *chuan(buf, s) = '\0';
  //chuan(buf, "hello");
  std::cout << buf << std::endl;

  expect<Eq>(digits(0xFFFFFFFFFFFFFFFFu), 20);
  expect<Eq>(digits(12345678901234567890u), 20);
  expect<Eq>(digits(10000000000000000000u), 20);
  expect<Eq>(digits(1234567890123456789u), 19);
  expect<Eq>(digits(1000000000000000000u), 19);
  expect<Eq>(digits(123456789012345678u), 18);
  expect<Eq>(digits(100000000000000000u), 18);
  expect<Eq>(digits(12345678901234567u), 17);
  expect<Eq>(digits(10000000000000000u), 17);
  expect<Eq>(digits(1234567890123456u), 16);
  expect<Eq>(digits(1000000000000000u), 16);
  expect<Eq>(digits(123456789012345u), 15);
  expect<Eq>(digits(100000000000000u), 15);
  expect<Eq>(digits(12345678901234u), 14);
  expect<Eq>(digits(10000000000000u), 14);
  expect<Eq>(digits(1234567890123u), 13);
  expect<Eq>(digits(1000000000000u), 13);
  expect<Eq>(digits(123456789012u), 12);
  expect<Eq>(digits(100000000000u), 12);
  expect<Eq>(digits(12345678901u), 11);
  expect<Eq>(digits(10000000000u), 11);
  expect<Eq>(digits(9234567890u), 10);
  expect<Eq>(digits(0xFFFFFFFFu), 10);

  expect<Eq>(digits(1234567890u), 10);
  expect<Eq>(digits(1000000000u), 10);
  expect<Eq>(digits(123456789u), 9);
  expect<Eq>(digits(100000000u), 9);
  expect<Eq>(digits(12345678u), 8);
  expect<Eq>(digits(10000000u), 8);
  expect<Eq>(digits(1234567u), 7);
  expect<Eq>(digits(1000000u), 7);
  expect<Eq>(digits(123456u), 6);
  expect<Eq>(digits(100000u), 6);
  expect<Eq>(digits(12345u), 5);
  expect<Eq>(digits(10000u), 5);
  expect<Eq>(digits(1234u), 4);
  expect<Eq>(digits(1000u), 4);
  expect<Eq>(digits(123u), 3);
  expect<Eq>(digits(100u), 3);
  expect<Eq>(digits(12u), 2);
  expect<Eq>(digits(10u), 2);
  expect<Eq>(digits(1u), 1);
  expect<Eq>(digits(0u), 1);
}

