#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>

template<typename T, std::size_t Size>
static constexpr unsigned getCombinationCount(const int value,
                                              const T(& denominations)[Size],
                                              const std::size_t index = Size)
{
  #define SELF(Value, Index) \
    getCombinationCount((Value), denominations, (Index))

  #define LHS SELF(value, (index - 1))
  #define RHS SELF((value - denominations[index - 1]), index)

  #define NEXT (LHS + RHS)

  return ((value <= 0) || !index) ? (!value) : (NEXT);

  #undef NEXT
  #undef RHS
  #undef LHS
  #undef SELF
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  static constexpr unsigned denominations[] = {1, 5, 10, 25, 50};

  // This problem's upper bound is quite small. Offloading the number crunching
  // to the compiler seems appropriate.
  static constexpr std::array<unsigned, 100> lut =
  {
    /*
      #!/usr/bin/env python3

      from decimal import Decimal

      for i in range(1, 101):
        end = ', ' if i % 5 else ',\n'
        print(('V({:.1e})'.format(Decimal(i))).replace('+', ''), end=end)
    */

    #define V(Value) ::getCombinationCount((Value), denominations)

    V(1.0e0), V(2.0e0), V(3.0e0), V(4.0e0), V(5.0e0),
    V(6.0e0), V(7.0e0), V(8.0e0), V(9.0e0), V(1.0e1),
    V(1.1e1), V(1.2e1), V(1.3e1), V(1.4e1), V(1.5e1),
    V(1.6e1), V(1.7e1), V(1.8e1), V(1.9e1), V(2.0e1),
    V(2.1e1), V(2.2e1), V(2.3e1), V(2.4e1), V(2.5e1),
    V(2.6e1), V(2.7e1), V(2.8e1), V(2.9e1), V(3.0e1),
    V(3.1e1), V(3.2e1), V(3.3e1), V(3.4e1), V(3.5e1),
    V(3.6e1), V(3.7e1), V(3.8e1), V(3.9e1), V(4.0e1),
    V(4.1e1), V(4.2e1), V(4.3e1), V(4.4e1), V(4.5e1),
    V(4.6e1), V(4.7e1), V(4.8e1), V(4.9e1), V(5.0e1),
    V(5.1e1), V(5.2e1), V(5.3e1), V(5.4e1), V(5.5e1),
    V(5.6e1), V(5.7e1), V(5.8e1), V(5.9e1), V(6.0e1),
    V(6.1e1), V(6.2e1), V(6.3e1), V(6.4e1), V(6.5e1),
    V(6.6e1), V(6.7e1), V(6.8e1), V(6.9e1), V(7.0e1),
    V(7.1e1), V(7.2e1), V(7.3e1), V(7.4e1), V(7.5e1),
    V(7.6e1), V(7.7e1), V(7.8e1), V(7.9e1), V(8.0e1),
    V(8.1e1), V(8.2e1), V(8.3e1), V(8.4e1), V(8.5e1),
    V(8.6e1), V(8.7e1), V(8.8e1), V(8.9e1), V(9.0e1),
    V(9.1e1), V(9.2e1), V(9.3e1), V(9.4e1), V(9.5e1),
    V(9.6e1), V(9.7e1), V(9.8e1), V(9.9e1), V(1.0e2)

    #undef V
  };

  for(unsigned n = 0; inputStream >> n;)
  {
    // As per the problem statement.
    assert((n >= 1) && (n <= lut.size()));

    std::cout << lut[n - 1] << '\n';
  }
}
