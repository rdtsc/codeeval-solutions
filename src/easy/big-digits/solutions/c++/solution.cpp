#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Slightly faster to split the digits beforehand than repetitive substr-ing.
  static const char* const art[] =
  {
    /*
      Transformations: /(.{5})/ -> "$1\\0"
                       /^|$/    -> "\""
                       /$/      -> ","
    */

    "-**--\0--*--\0***--\0***--\0-*---\0****-\0-**--\0****-\0-**--\0-**--",
    "*--*-\0-**--\0---*-\0---*-\0*--*-\0*----\0*----\0---*-\0*--*-\0*--*-",
    "*--*-\0--*--\0-**--\0-**--\0****-\0***--\0***--\0--*--\0-**--\0-***-",
    "*--*-\0--*--\0*----\0---*-\0---*-\0---*-\0*--*-\0-*---\0*--*-\0---*-",
    "-**--\0-***-\0****-\0***--\0---*-\0***--\0-**--\0-*---\0-**--\0-**--",
    "-----\0-----\0-----\0-----\0-----\0-----\0-----\0-----\0-----\0-----"
  };

  static const std::size_t digitHeight = sizeof art / sizeof *art,
                           digitWidth  = 6;

  for(std::string line, digits; std::getline(inputStream, line); digits.clear())
  {
    std::copy_if(line.cbegin(), line.cend(),
                 std::back_inserter(digits), ::isdigit);

    if(digits.empty()) continue;

    // Jump to the start of the current digit in scanline Y and print the
    // next 5 characters.
    for(auto y = decltype(digitHeight){}; y < digitHeight; ++y)
    {
      for(const auto c : digits)
      {
        std::cout << (art[y] + (c - '0') * digitWidth);
      }

      std::cout << '\n';
    }
  }
}
