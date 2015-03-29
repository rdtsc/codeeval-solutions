#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::size_t length = 0;
  std::string alphabet;
  std::vector<std::size_t> mask;

  while(inputStream >> length >> alphabet)
  {
    assert(length && alphabet.front() == ',');

    mask.clear();

    // Remove the comma.
    alphabet.erase(0, 1);

    std::sort(alphabet.begin(), alphabet.end());

    const std::size_t uniqueLetterCount = [&]
    {
      const auto newEnd = std::unique(alphabet.begin(), alphabet.end());

      return std::distance(alphabet.begin(), newEnd);
    }();

    [&]
    {
      for(mask.resize(length);;)
      {
        for(const auto i : mask) std::cout << alphabet[i];

        // Create an index mask in the appropriate base.
        for(int i = length - 1;; --i)
        {
          if(i < 0) return;

          else if(++mask[i] == uniqueLetterCount) mask[i] = 0;

          else
          {
            std::cout << ',';
            break;
          }
        }
      }
    }();

    std::cout << '\n';
  }
}
