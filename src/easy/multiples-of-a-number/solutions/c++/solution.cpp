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

  char ignore = '\0';

  for(unsigned x = 0, n = 0; inputStream >> x >> ignore >> n;)
  {
    decltype(x) exponent = 1;

    // Get N's exponent; N = 2^exponent
    for(auto m = n; m > 2; m >>= 1) ++exponent;

    // Divide X by N.
    auto multiple = (x >> exponent);

    // Was the division clean?
    if(multiple * n != x) ++multiple;

    std::cout << (n * multiple) << '\n';
  }
}
