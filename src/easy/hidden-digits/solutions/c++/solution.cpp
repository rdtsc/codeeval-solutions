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

  for(std::string line, result; std::getline(inputStream, line); result.clear())
  {
    for(const auto c : line)
    {
      if(::isdigit(c)) result.push_back(c);

      // Map [a, j] -> [0, 9].
      else if(c >= 'a' && c <= 'j') result.push_back('0' + (c - 'a'));
    }

    std::cout << (result.empty() ? "NONE" : result) << '\n';
  }
}
