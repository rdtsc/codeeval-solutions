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

  for(unsigned n = 0; inputStream >> n;)
  {
    decltype(n) result = 0;

    for(const auto denomination : {5, 3})
    {
      result += (n / denomination);
      n -= (n / denomination) * denomination;
    }

    std::cout << (result + n) << '\n';
  }
}
