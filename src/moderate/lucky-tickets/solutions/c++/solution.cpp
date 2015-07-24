#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Upper bound is small. Precomputed LUT for speed.
  static const char* const sequence[] =
  {
    /*
      As described in: OEIS A025015 by Warut Roonguthai -- Number of integers in
      [0, 10^n-1] whose sums of digits are equal to the most common value, which
      is 9*n/2 for even n and (9*n +/- 1)/2 for odd n > 1. E.g. The most common
      value of sums of digits of numbers from 0 to 9999 is 9*4/2 = 18, so there
      are a(4)=670 numbers in this range whose sums of digits are 18.
    */

    /*
      #!/usr/bin/env python

      from math import factorial as factorial
      from math import floor as floor

      def count(n):
        sum = 0

        nCk = lambda n, k: factorial(n) // (factorial(k) * factorial(n - k))

        for k in range(0, int(floor(9 * n / 20) + 1)):
          x = (-1) ** k
          N = n + floor(9 * n / 2) - (10 * k) - 1
          K = n - 1

          sum += x * nCk(n, k) * nCk(N, K)

        return sum

      for n in range(2, 101, 2):
        print('\x2F* {:3d} *\x2F "{:d}",'.format(n, count(n)))
    */

    #include "sequence.dat"
  };

  static const auto sequenceSize = (sizeof sequence / sizeof *sequence);

  for(unsigned n = 0; inputStream >> n;)
  {
    assert(!(n & 1) && (n <= (sequenceSize * 2)));

    std::cout << sequence[(n / 2) - 1] << '\n';
  }
}
