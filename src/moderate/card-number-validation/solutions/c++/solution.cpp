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
    // Sum of the card digits after applying the Luhn algorithm.
    const auto digitChecksum = [&]
    {
      unsigned result = 0;
      std::size_t index = 0;

      // Read card digits RTL.
      std::for_each(line.rbegin(), line.rend(), [&](const char c)
      {
        if(!::isdigit(c)) return;

        // Odd digits are added to the checksum verbatim.
        else if(++index & 1) result += c - '0';

        else
        {
          // Even digits are doubled...
          const auto digit = (c - '0') * 2;

          // ...and if the result is >= 10, the resulting digits are
          // summed together and added to the checksum.
          result += (digit >= 10) ? (digit - 9) : (digit);
        }
      });

      return result;
    }();

    // Card number is considered valid if its checksum is a multiple of 10.
    std::cout << !(digitChecksum % 10) << '\n';
  }
}
