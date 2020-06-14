#include <chrono>
#include <cstdio>
#include <ios>
#include <iostream>

int main() {
  constexpr float cf = 0xffffffff;
  const int cn = (int)cf;
  using clock = std::chrono::system_clock;
  float df = 1e100 < (clock::now() - clock::now()).count() ? 1234.0f : cf;
  int dn = (int)df;

  printf("cn=%x  (int)cf=%x  dn=%x  (int)df=%x\n", cn, (int)cf, dn, (int)df);
  std::cout << std::hex;
  std::cout                      //
      << "cn=" << cn             //
      << "  (int)cf=" << (int)cf //
      << "  dn=" << dn           //
      << "  (int)df=" << (int)df << std::endl;
}
