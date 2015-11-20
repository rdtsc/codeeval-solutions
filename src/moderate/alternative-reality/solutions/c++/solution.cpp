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

static constexpr unsigned denominations[] = {1, 5, 10, 25, 50};

static constexpr unsigned operator"" _(const unsigned long long n)
{
  return ::getCombinationCount(n, denominations);
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // This problem's upper bound is quite small. Offloading the number crunching
  // to the compiler seems appropriate.
  static constexpr std::array<unsigned, 100> lut =
  {
    /*
      #!/usr/bin/env python3

      for i in range(1, 101):
        print('{:>2}_'.format(i), end=', ' if i % 10 else ',\n')
    */

     1_,  2_,  3_,  4_,  5_,  6_,  7_,  8_,  9_, 10_,
    11_, 12_, 13_, 14_, 15_, 16_, 17_, 18_, 19_, 20_,
    21_, 22_, 23_, 24_, 25_, 26_, 27_, 28_, 29_, 30_,
    31_, 32_, 33_, 34_, 35_, 36_, 37_, 38_, 39_, 40_,
    41_, 42_, 43_, 44_, 45_, 46_, 47_, 48_, 49_, 50_,
    51_, 52_, 53_, 54_, 55_, 56_, 57_, 58_, 59_, 60_,
    61_, 62_, 63_, 64_, 65_, 66_, 67_, 68_, 69_, 70_,
    71_, 72_, 73_, 74_, 75_, 76_, 77_, 78_, 79_, 80_,
    81_, 82_, 83_, 84_, 85_, 86_, 87_, 88_, 89_, 90_,
    91_, 92_, 93_, 94_, 95_, 96_, 97_, 98_, 99_, 100_
  };

  for(unsigned n = 0; inputStream >> n;)
  {
    // As per the problem statement.
    assert((n >= 1) && (n <= lut.size()));

    std::cout << lut[n - 1] << '\n';
  }
}
