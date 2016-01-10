#include <array>
#include <cassert>
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

  std::array<char, 1024> match = {'\0'};

  for(std::string line; std::getline(inputStream, line);)
  {
    const auto foundMatch = [&]
    {
      const auto width = line.size();

      assert(width < (match.size() - 1));

      // The supplied strings are pretty short in this challenge.
      // Dumb brute force should be adequate.
      for(auto i = width; i; --i)
      for(auto j = decltype(i){}; j < (width - i); ++j)
      {
        // As per the problem statement.
        if(!line.compare(j, i, " ")) continue;

        // Save candidate.
        match[line.copy(match.data(), i, j)] = '\0';

        const auto cursor = line.find(match.data());

        // Found a dupe.
        if(line.find(match.data(), (cursor + i)) != std::string::npos)
        {
          return true;
        }
      }

      return false;
    }();

    std::cout << (foundMatch ? match.data() : "NONE") << '\n';
  }
}
