#include <cassert>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::unordered_map<char, std::size_t> hitCount;

  for(std::string line; std::getline(inputStream, line); hitCount.clear())
  {
    // Count the number of occurrences of each character.
    for(const auto c : line) if(::isalpha(c))
    {
      ++hitCount[c];
    }

    // Dump the first character that's unique.
    for(const auto c : line) if(::isalpha(c) && hitCount[c] == 1)
    {
      std::cout << c << '\n';
      break;
    }
  }
}
