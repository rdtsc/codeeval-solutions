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
    const auto end   = line.find_last_of(' '),
               begin = line.find_last_of(' ', end - 1);

    if(begin != std::string::npos)
    {
      std::cout << line.substr(begin + 1, end - begin - 1);
    }

    else if(end != std::string::npos)
    {
      std::cout << line.substr(0, end);
    }

    else std::cout << line;
  }
}
