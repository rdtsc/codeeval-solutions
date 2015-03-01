#include <cassert>
#include <cctype>
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

  for(std::string line; std::getline(inputStream, line); std::cout << '\n')
  {
    bool hasLetter['z' - 'a' + 1] = {};

    for(const auto c : line) if(::isalpha(c))
    {
      hasLetter[::tolower(c) - 'a'] = true;
    }

    auto isPangram = true;

    for(std::size_t i = 0; i < sizeof hasLetter / sizeof *hasLetter; ++i)
    {
      if(!hasLetter[i])
      {
        isPangram = false;
        std::cout << static_cast<char>(i + 'a');
      }
    }

    if(isPangram) std::cout << "NULL";
  }
}
