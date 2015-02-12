#include <algorithm>
#include <cassert>
#include <cctype>
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

  std::vector<float> numbers;

  for(std::string line; std::getline(inputStream, line); numbers.clear())
  {
    // Process the expression RTL.
    std::for_each(line.rbegin(), line.rend(), [&numbers](const char c)
    {
      if(::isdigit(c)) numbers.push_back(c - '0');

      else if(c != ' ') switch(c)
      {
        #define HANDLE_OP(op) case *(#op): { \
          const auto x = numbers.back(); numbers.pop_back(); \
          const auto y = numbers.back(); numbers.pop_back(); \
          numbers.push_back(x op y); \
        } break

        HANDLE_OP(*); HANDLE_OP(+); HANDLE_OP(/); HANDLE_OP(-);

        #undef HANDLE_OP
      }
    });

    std::cout << static_cast<unsigned>(numbers.back()) << '\n';
  }
}
