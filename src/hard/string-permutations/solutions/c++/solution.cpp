#include <algorithm>
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

  for(std::string line; std::getline(inputStream, line); std::cout << '\n')
  {
    const auto lineBegin = line.begin(), lineEnd = line.end();

    std::sort(lineBegin, lineEnd);

    for(std::cout << line; std::next_permutation(lineBegin, lineEnd);)
    {
      std::cout << ',' << line;
    }
  }
}
