#include <cstdio>
#include <ios>
#include <iostream>

int main() {
  float f = 0xffffffff;
  int n = (int)f;
  printf("(int)%.1f == %d, %d\n", f, n, (int)f);
  std::cout << std::fixed             //
            << "(int)" << f << " == " //
            << n << ", " << (int)f    //
            << std::endl;
}
