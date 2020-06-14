#include <iostream>

#include <limits>
#include <type_traits>

namespace can_represent_by_impl {
template <typename to_type_, typename from_type_> //
struct T {
  using to_type = to_type_;
  using from_type = from_type_;
  using from_lim = std::numeric_limits<from_type>;
  using to_lim = std::numeric_limits<to_type>;
  static_assert(from_lim::is_iec559, "from_type should be IEEE754 type");
  static_assert(from_lim::radix == 2, "radix should be 2");
  static_assert(to_lim::is_integer, "to_type should be integer");
  static_assert(to_lim::radix == 2, "radix should be 2");
  static constexpr int diff_digits() {
    return to_lim::digits - from_lim::digits;
  }
  static constexpr to_type raw_lo() { return to_lim::lowest(); }
  static constexpr to_type raw_hi() { return to_lim::max(); }
  static constexpr to_type mask() {
    return 0 < diff_digits() //
               ? ~((to_type(1) << diff_digits()) - 1)
               : ~to_type(0);
  }
  static constexpr to_type lo() { return raw_lo() & mask(); }
  static constexpr to_type hi() { return raw_hi() & mask(); }
};

} // namespace can_represent_by_impl

template <typename to_type, typename from_type>
constexpr bool //
can_represent_by(from_type v) {
  using t = can_represent_by_impl::T<to_type, from_type>;
  return t::lo() <= v && v <= t::hi() && v == static_cast<to_type>(v);
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
  float f2_32 = float(1ULL << 32);
  float f2_31 = float(1ULL << 31);
  test<std::uint32_t, float>( //
      f2_32, false, "(float)(2**32) -> uint32_t");
  test<std::uint32_t, float>( //
      0.0f, true, "-0.0f -> uint32_t");
  test<std::uint32_t, float>( //
      std::nextafterf(0.0f, -1e30), false, "prev of 0.0f -> uint32_t");
  test<std::uint64_t, float>( //
      f2_32 * f2_32, false, "(float)(2**64) -> uint64_t");
  test<std::int32_t, float>( //
      f2_31, false, "(float)(2**31) -> int32_t");
  test<std::int32_t, float>( //
      -f2_31, true, "(float)(-2**31) -> int32_t");
  test<std::int32_t, float>( //
      std::nextafterf(-f2_31, -1e30), false,
      "prev of (float)(-2**31) -> int32_t");
  test<std::int32_t, float>( //
      std::nextafterf(-f2_31, 1e30), true,
      "next of (float)(-2**31) -> int32_t");
  test<std::int32_t, float>( //
      std::nextafterf(f2_31, 1e30), false, "next of (float)(2**31) -> int32_t");
}
