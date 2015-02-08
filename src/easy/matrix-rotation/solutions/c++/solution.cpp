#include <cassert>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(std::string line; std::getline(inputStream, line);)
  {
    static auto resultBuffer = []
    {
      std::string result;

      result.reserve(220);

      return result;
    }();

    resultBuffer.clear();

    const auto lineLength = line.size();

    assert(lineLength);

    // Elements in the matrix.
    const std::size_t elementCount = (lineLength / 2) + 1;

    // N in the N-by-N matrix.
    const std::size_t n = std::sqrt(elementCount);

    assert(n * n == elementCount);

    for(std::size_t i = 0; i < n; ++i)
    {
      for(std::size_t j = 1; j <= n; ++j)
      {
        /*
          |0 1 2|    |6 3 0|
          |3 4 5| -> |7 4 1| -> "6 3 0 7 4 1 8 5 2"
          |6 7 8|    |8 5 2|

          L = 9; element count.
          N = 3; side length.

          '6' -> (L - 1N + 0)  |  '3' -> (L - 2N + 0)  |  '0' -> (L - 3N + 0)
          '7' -> (L - 1N + 1)  |  '4' -> (L - 2N + 1)  |  '1' -> (L - 3N + 1)
          '8' -> (L - 1N + 2)  |  '5' -> (L - 2N + 2)  |  '2' -> (L - 3N + 2)
        */

        (resultBuffer += line[(elementCount - (n * j) + i) * 2]) += ' ';
      }
    }

    std::cout << resultBuffer << '\n';
  }
}
