#include <cassert>
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
    unsigned upperBound = std::stoi(line), lowerBound = 0, guess = 0;

    // Adjust ballpark based on responses.
    for(std::string::size_type i = line.find_first_of("HLY");
        i != std::string::npos; i = line.find_first_of("HLY", i + 1))
    {
      const auto delta = upperBound - lowerBound;

      guess = lowerBound + (delta & 1) + (delta / 2);

      switch(line[i])
      {
        case 'H': lowerBound = guess + 1; break;
        case 'L': upperBound = guess - 1; break;
      }
    }

    std::cout << guess << '\n';
  }
}
