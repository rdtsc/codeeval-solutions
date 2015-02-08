#include <algorithm>
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

  using input_iterator  = std::istream_iterator<unsigned>;
  using output_iterator = std::ostream_iterator<bool>;

  std::transform(input_iterator(inputStream),
                 input_iterator(),
                 output_iterator(std::cout, "\n"),
                 [](const input_iterator::value_type n) {return !(n & 1);});
}
