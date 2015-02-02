#include <cassert>
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

  for(unsigned x = 0, y = 0, n = 0; inputStream >> x >> y >> n;)
  {
    for(decltype(n) i = 1; i <= n; ++i)
    {
      const bool isF = !(i % x), // 'F' for numbers divisible by X.
                 isB = !(i % y); // 'B' for numbers divisible by Y.

      if(isF && isB) std::cout << "FB";
      else if(isF)   std::cout << 'F';
      else if(isB)   std::cout << 'B';
      else           std::cout << i;

      // Looks like CE doesn't care about trailing spaces. Whatever.
      std::cout << ' ';
    }

    std::cout << '\n';
  }
}
