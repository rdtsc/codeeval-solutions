#include <algorithm>
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

  const auto decode = [](std::string& message)
  {
    for(char& c : message) c = "uvwxyznopqrstghijklmabcdef"[c - 'a'];
  };

  for(std::string line; std::getline(inputStream, line);)
  {
    // As per the problem statement.
    assert(std::all_of(line.cbegin(), line.cend(), [](const char c)
    {
      return ::isalpha(c) && ::islower(c);
    }));

    decode(line);

    std::cout << line << '\n';
  }
}
