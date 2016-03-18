#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  static const unsigned terms[] = {3, 7, 31, 127, 2047};

  static const auto termsBegin = std::begin(terms),
                    termsEnd   = std::end(terms);

  for(unsigned n = 0; inputStream >> n; std::cout << "\n")
  {
    std::cout << *termsBegin;

    for(auto i = std::next(termsBegin); i != termsEnd; ++i)
    {
      if(*i >= n) break;

      std::cout << ", " << *i;
    }
  }
}
