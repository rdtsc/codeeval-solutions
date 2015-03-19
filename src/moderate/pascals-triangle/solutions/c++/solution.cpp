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
    Looks like we're dealing with triangles whose depths max out at 20. It
    therefore makes sense from a performance standpoint to pre-calculate and
    format the sequence up to this depth and simply dump the results on demand.

                                      1
                                    1   1
                                  1   2   1
                                 1  3   3   1
                               1  4   6   4   1
                                     ^^^

    Indexing into Pascal's triangle (as above) can be done via n-choose-k:

                 N: row; K: column    |    N: 4; K: 2
                                      |
                        n!            |        4!
                    ----------        |    ----------  =  6
                    k!(n - k)!        |    2!(4 - 2)!
  */

  char sequenceBuffer[] =
  {
    /*
      #!/usr/bin/env python

      import math

      f = math.factorial
      nCk = lambda n, k: f(n) // (f(k) * f(n - k))

      seq = [nCk(d, i) for d in range(0, 21) for i in range(0, d + 1)]
      seq = ''.join(str(seq).strip('[]').split(','))
      print('"' + seq + '"')
    */

    #include "sequence.dat"
  };

  static const std::size_t breakpoints[] =
  {
    /*
      ...

      import re

      row = [m.start() + 1 for m in re.finditer(r'\b1 1\b', seq)]
      print(str(row).strip('[]'))
    */

    #include "breakpoints.dat"
  };

  static const std::size_t breakpointCount =
    sizeof breakpoints / sizeof *breakpoints;

  for(unsigned depth = 0; inputStream >> depth;)
  {
    assert(depth && depth <= (breakpointCount + 1));

    // Temporarily snip the end of the sequence, if applicable. Should be a bit
    // faster than substr-ing an std::basic_string.
    if(depth != breakpointCount + 1)
    {
      assert(sequenceBuffer[breakpoints[depth - 1]] == ' ');
      sequenceBuffer[breakpoints[depth - 1]] = '\0';
    }

    std::cout << sequenceBuffer << '\n';

    // Glue the sequence back together, if applicable.
    if(depth != breakpointCount + 1)
    {
      assert(!sequenceBuffer[breakpoints[depth - 1]]);
      sequenceBuffer[breakpoints[depth - 1]] = ' ';
    }
  }
}
