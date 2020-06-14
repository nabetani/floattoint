#include <cmath>
#include <cstdio>
#include <iomanip>
#include <ios>
#include <iostream>

#include <cmath>
bool is_int_lround(double x) { return std::lround(x) == x; }

double prev(double x) { return std::nextafter(x, -1e100); }
double next(double x) { return std::nextafter(x, 1e100); }

void test(double x, char const *msg) {
  printf("%s (%.1f):", msg, x);
  std::cout << is_int_lround(x) << std::endl;
}

int main() {
  printf("%lx\n", lround(std::pow(2.0, 64.0)));

  double b32 = (1ULL << 32);
  double b31 = (1ULL << 31);
  double b64 = b32 * b32;
  double b63 = b32 * b31;
  std::cout << std::boolalpha;
  test(b32, "2**32");
  test(b64, "2**64");
  test(b63, "2**63");
  test(prev(b63), "prev(2**63)");
}