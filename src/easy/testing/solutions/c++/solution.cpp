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
    const auto marker = line.find('|');

    assert((marker != std::string::npos) && (marker >= 2));

    const auto mistakes = [&]
    {
      std::size_t result = 0;

      for(std::size_t i = 0; i < (marker - 1); ++i)
      {
        if(line[i] != line[i + marker + 2]) ++result;
      }

      return result;
    }();

    const char* const priority = mistakes == 0 ? "Done"    :
                                 mistakes <= 2 ? "Low"     :
                                 mistakes <= 4 ? "Medium"  :
                                 mistakes <= 6 ? "High"    :
                                                 "Critical";

    std::cout << priority << '\n';
  }
}
