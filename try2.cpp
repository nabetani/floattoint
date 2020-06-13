#include <iostream>

#include <limits>
#include <type_traits>

template <typename to_type, typename from_type>
bool //
can_represent_by(from_type v) {
  using from_lim = std::numeric_limits<from_type>;
  using to_lim = std::numeric_limits<to_type>;
  static_assert(from_lim::is_iec559, "from_type should be IEEE754 type");
  static_assert(from_lim::radix == 2 && to_lim::radix == 2,
                "radix should be 2");
  auto diff_digits = to_lim::digits - from_lim::digits;
  auto raw_lo = to_lim::lowest();
  auto raw_hi = to_lim::max();
  auto mask = 0 < diff_digits //
                  ? ~((to_type(1) << diff_digits) - 1)
                  : ~to_type(0);
  auto lo = raw_lo & mask;
  auto hi = raw_hi & mask;
  printf("%f %f %30.15e %x\n", (double)lo, (double)hi, (double)v,
         (uint32_t)mask);
  if (v < lo || hi < v) {
    return false;
  }
  std::cout << "use cast\n";
  return v == static_cast<to_type>(v);
}

#include <cmath>
#include <cstdio>
#include <ios>

template <typename to_type, typename from_type>
void test(from_type v, bool expected, char const *msg) {
  bool actual = can_represent_by<to_type>(v);
  std::cout                                     //
      << (actual == expected ? "ok" : "**NG**") //
      << std::fixed                             //
      << "  v=" << v << " "                     //
      << msg << std::endl;
}

int main() {
  test<std::uint32_t, float>( //
      (float)(1ULL << 32), false, "(float)(1ULL << 32) -> uint32_t");
  test<std::uint32_t, float>( //
      0.0f, true, "-0.0f -> uint32_t");
  test<std::uint32_t, float>( //
      std::nextafterf(0.0f, -1e30), false, "prev of 0.0f -> uint32_t");
  test<std::uint64_t, float>( //
      (float)(1ULL << 32) * (float)(1ULL << 32), false,
      "(float)(2**64) -> uint64_t");
  test<std::int32_t, float>( //
      (float)(0x7fffffff), false, "(float)(0x7fffffff) -> int32_t");
  test<std::int32_t, float>( //
      (float)(-0x80000000LL), true, "(float)(-0x80000000) -> int32_t");
  test<std::int32_t, float>( //
      std::nextafterf((float)(-0x80000000LL), -1e30), false,
      "prev of (float)(-0x80000000) -> int32_t");
  test<std::int32_t, float>( //
      std::nextafterf((float)(-0x80000000LL), 1e30), true,
      "next of (float)(-0x80000000) -> int32_t");
  test<std::int32_t, float>( //
      std::nextafterf((float)(0x7fffffff), 1e30), false,
      "next of (float)(0x7fffffff) -> int32_t");
}
