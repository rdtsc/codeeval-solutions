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
    // N falls within the range of [0, 100].
    unsigned count[101] = {};
    std::size_t termCount = 0;

    // Count term occurances.
    {
      std::size_t term = 0;

      const auto lineSize = line.size();

      for(auto i = decltype(lineSize){}; i < lineSize; ++i)
      {
        const char c = line[i];

        // Reconstruct the term.
        if(c >= '0' && c <= '9')
        {
          term = (term * 10) + (c - '0');
          continue;
        }

        // Term boundary.
        ++count[term];
        ++termCount;
        term = 0;
      }

      // Account for the last term.
      ++count[term];
      ++termCount;
    }

    bool hasMajorElement = false;

    // "Major" elements appear more than L/2 times in the sequence of terms.
    // A bit faster than std::find_if on CE.
    for(std::size_t i = 0; i < sizeof count / sizeof *count; ++i)
    {
      if(count[i] > (termCount / 2))
      {
        std::cout << i << '\n';
        hasMajorElement = true;
        break;
      }
    }

    if(!hasMajorElement) std::cout << "None\n";
  }
}
