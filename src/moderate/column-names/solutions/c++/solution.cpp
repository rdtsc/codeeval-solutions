#include <cassert>
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

  std::string result;

  for(unsigned n = 0; inputStream >> n; result.clear())
  {
    // Emit characters backwards.
    for(;n--; n /= 26) result.insert(0, 1, 'A' + (n % 26));

    std::cout << result << '\n';
  }
}
