#include <algorithm>
#include <cassert>
#include <cctype>
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
    assert(std::count_if(line.cbegin(), line.cend(), ::isdigit) == 16);
    assert(std::count_if(line.cbegin(), line.cend(), ::isspace) ==  3);
    assert(line.find_first_not_of("0123456789 ") == std::string::npos);

    const auto tally = [&]
    {
      unsigned result = 0;
      std::size_t digitIndex = 0;

      for(const auto c : line) if(::isdigit(c))
      {
        result += (c - '0') * ((++digitIndex & 1) + 1);
      }

      return result;
    }();

    std::cout << (tally % 10 ? "Fake" : "Real") << '\n';
  }
}
