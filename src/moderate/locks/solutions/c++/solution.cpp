#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::vector<bool> locks;

  locks.reserve(1000);

  for(unsigned doorCount = 0, iterations = 0;
      inputStream >> doorCount >> iterations; locks.clear())
  {
    locks.resize(doorCount);

    #define EVERY(Index, Counter)                            \
      for(auto (Counter) = decltype(doorCount){(Index) - 1}; \
      (Counter) < doorCount; (Counter) += (Index))

    #define LOCK(Index) locks[(Index)] = true
    #define FLIP(Index) locks[(Index)] = !locks[(Index)]

    for(auto i = decltype(iterations){1}; i < iterations; ++i)
    {
      EVERY(2, j) LOCK(j);
      EVERY(3, j) FLIP(j);
    }

    // Last iteration consists of a single op.
    FLIP(doorCount - 1);

    #undef FLIP
    #undef LOCK
    #undef EVERY

    std::cout << std::count(locks.cbegin(), locks.cend(), false) << '\n';
  }
}
