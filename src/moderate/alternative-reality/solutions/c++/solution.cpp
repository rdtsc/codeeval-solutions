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

  #define LHS() SELF(value, (index - 1))
  #define RHS() SELF((value - denominations[index - 1]), index)

  return ((value <= 0) || !index) ? (!value) : (LHS() + RHS());

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
        print('V(0x{:02X})'.format(i), end=', ' if i % 5 else ',\n')
    */

    #define V(Value) ::getCombinationCount((Value), denominations)

    V(0x01), V(0x02), V(0x03), V(0x04), V(0x05),
    V(0x06), V(0x07), V(0x08), V(0x09), V(0x0A),
    V(0x0B), V(0x0C), V(0x0D), V(0x0E), V(0x0F),
    V(0x10), V(0x11), V(0x12), V(0x13), V(0x14),
    V(0x15), V(0x16), V(0x17), V(0x18), V(0x19),
    V(0x1A), V(0x1B), V(0x1C), V(0x1D), V(0x1E),
    V(0x1F), V(0x20), V(0x21), V(0x22), V(0x23),
    V(0x24), V(0x25), V(0x26), V(0x27), V(0x28),
    V(0x29), V(0x2A), V(0x2B), V(0x2C), V(0x2D),
    V(0x2E), V(0x2F), V(0x30), V(0x31), V(0x32),
    V(0x33), V(0x34), V(0x35), V(0x36), V(0x37),
    V(0x38), V(0x39), V(0x3A), V(0x3B), V(0x3C),
    V(0x3D), V(0x3E), V(0x3F), V(0x40), V(0x41),
    V(0x42), V(0x43), V(0x44), V(0x45), V(0x46),
    V(0x47), V(0x48), V(0x49), V(0x4A), V(0x4B),
    V(0x4C), V(0x4D), V(0x4E), V(0x4F), V(0x50),
    V(0x51), V(0x52), V(0x53), V(0x54), V(0x55),
    V(0x56), V(0x57), V(0x58), V(0x59), V(0x5A),
    V(0x5B), V(0x5C), V(0x5D), V(0x5E), V(0x5F),
    V(0x60), V(0x61), V(0x62), V(0x63), V(0x64),

    #undef V
  };

  for(unsigned n = 0; inputStream >> n;)
  {
    // As per the problem statement.
    assert((n >= 1) && (n <= lut.size()));

    std::cout << lut[n - 1] << '\n';
  }
}
