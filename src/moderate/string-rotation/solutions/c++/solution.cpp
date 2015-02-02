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
    /*
       lhs   rhs
        |     |
      Hello,lloHe

      lhs+lhs = HelloHello
                  \___/
                    |
                   rhs
    */

    const auto lineMidpoint = line.size() / 2;

    // Left and right chunks should be the same length.
    if(line[lineMidpoint] != ',')
    {
      std::cout << "False\n";
      continue;
    }

    // Locate RHS in LHS+LHS.
    const bool isSubstring = [&]
    {
      const auto lhs = line.substr(0, lineMidpoint),
                 rhs = line.substr(lineMidpoint + 1);

      return (lhs + lhs).find(rhs) != std::string::npos;
    }();

    std::cout << (isSubstring ? "True" : "False") << '\n';
  }
}
