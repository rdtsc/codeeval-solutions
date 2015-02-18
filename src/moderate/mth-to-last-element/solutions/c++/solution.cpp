#include <algorithm>
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
    // Number of character elements. This is directly proportional to the
    // number of spaces in the input string.
    const auto n = std::count(line.cbegin(), line.cend(), (std::size_t)' ');

    const auto mOffset = line.find_last_of(' ');

    // Mth element that we're supposed to output.
    const auto m = decltype(n){std::stoi(line.substr(mOffset))};

    if(n >= m) std::cout << line[(n - m) * 2] << '\n';
  }
}
