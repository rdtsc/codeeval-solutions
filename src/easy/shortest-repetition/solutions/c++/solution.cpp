#include <algorithm>
#include <bitset>
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

  static const auto isPrime = []
  {
    std::bitset<100> result;

    const auto primes =
    {
      02, 03, 05, 07, 11,
      13, 17, 19, 23, 29,
      31, 37, 41, 43, 47,
      53, 59, 61, 67, 71,
      73, 79, 83, 89, 97,
    };

    for(const auto prime : primes) result.set(prime);

    return result;
  }();

  for(std::string line; std::getline(inputStream, line); std::cout << '\n')
  {
    const std::size_t lineLength = line.size();

    assert(lineLength <= 80);

    // Check if the input consists of a single repeating character.
    const bool isHomogeneous =
      (line.find_first_not_of(line.front()) == std::string::npos);

    if(isHomogeneous)
    {
      std::cout << '1';
      continue;
    }

    else if(isPrime[lineLength])
    {
      std::cout << lineLength;
      continue;
    }

    auto index = decltype(lineLength){2};

    // Check blocks of increasing size until we find the first
    // repetition period or exhaust the string.
    const bool foundMatch = [&]
    {
      for(;index <= (lineLength / 2); ++index)
      {
        bool foundMatch = false;

        for(auto j = index; j < lineLength; j += index)
        {
          if(line.compare(j, index, line, 0, index))
          {
            foundMatch = false;
            break;
          }

          foundMatch = true;
        }

        if(foundMatch) return true;
      }

      return false;
    }();

    std::cout << (foundMatch ? index : lineLength);
  }
}
