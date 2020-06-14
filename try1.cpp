#include <limits>
#include <type_traits>

template <typename to_type, typename from_type>
constexpr bool //
can_represent_by(from_type v) {
  using to_lim = std::numeric_limits<to_type>;
  if (v < to_lim::lowest() || to_lim::max() < v) {
    return false;
  }
  return static_cast<to_type>(v) == v;
}

#include <cstdio>
#include <ios>
#include <iostream>

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
  test<std::uint32_t, float>((float)(1ULL << 32), false,
                             "(float)(1ULL << 32) -> uint32_t");
}
