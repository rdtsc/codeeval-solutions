#include <algorithm>
#include <cassert>
#include <cstdint>
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

  std::uint_fast64_t result = 0;

  for(std::string flag, sequence; inputStream >> flag >> sequence;)
  {
    // "00" indicates that the following sequence of zeros should be
    // "transformed" into ones.
    const bool shouldInvertSequence = (flag.size() == 2);

    std::for_each(sequence.cbegin(), sequence.cend(), [&](const char)
    {
      result <<= 1;
      result += shouldInvertSequence;
    });

    switch(inputStream.peek()) case '\n': case EOF:
    {
      std::cout << result << '\n';
      result = 0;
    }
  }
}
