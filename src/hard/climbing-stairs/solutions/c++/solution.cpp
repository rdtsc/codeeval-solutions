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

  /*
    Stairs  ->                          Possibilities
    ------      -------------------------------------------------------------
      1         [1]: (1)
      2         [2]: (1+1), (2)
      3         [3]: (1+1+1), (1+2), (2+1)
      4         [5]: (1+1+1+1), (1+1+2), (1+2+1), (2+1+1), (2+2)
      5         [8]: (1+1+1+1+1), (1+1+1+2), (1+1+2+1), (1+2+1+1), (2+1+1+1),
                     (2+2+1), (2+1+2), (1+2+2)
  */

  // The upper limit is 1k stairs. A precomputed LUT should be OK.
  // Might have to write a BigInt string adder later.
  static const char* const sequence[] =
  {
    /*
      #!/usr/bin/env python

      def dump(n, index):
        print('\x2F* ', index ,' *\x2F "', n, '"', sep='', end=',\n')

      x = 1
      y = 1

      dump(0, 0)
      dump(x, 1)
      dump(y, 2)

      for i in range(3, 1002):
        z = x + y
        x = y
        y = z
        dump(z, i)
    */

    #include "sequence.dat"
  };

  for(std::size_t n = 0; inputStream >> n;)
  {
    assert(n && ((n + 1) < sizeof sequence / sizeof *sequence));

    std::cout << sequence[n + 1] << '\n';
  }
}
