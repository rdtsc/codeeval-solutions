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

  for(std::string line; std::getline(inputStream, line);)
  {
    if(line.empty()) continue;

    const auto delimiterPos = line.rfind(',');

    assert(delimiterPos != std::string::npos);

    const auto lhs = line.substr(0, delimiterPos),
               rhs = line.substr(delimiterPos + 1);

    const bool isSubstring = [&]
    {
      const auto lhsSize = lhs.size(),
                 rhsSize = rhs.size();

      if(rhsSize > lhsSize) return false;

      return lhs.find(rhs, lhsSize - rhsSize) != std::string::npos;
    }();

    std::cout << isSubstring << '\n';
  }
}
