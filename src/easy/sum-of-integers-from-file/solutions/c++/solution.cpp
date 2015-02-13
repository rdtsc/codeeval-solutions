#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  using input_iterator = std::istream_iterator<unsigned>;

  const auto tally = std::accumulate(input_iterator(inputStream),
                                     input_iterator(),
                                     input_iterator::value_type());

  std::cout << tally << '\n';
}
