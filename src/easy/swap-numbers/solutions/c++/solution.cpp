#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::istringstream tokenizer;

  for(std::string line; std::getline(inputStream, line); std::cout << '\n')
  {
    tokenizer.clear();
    tokenizer.str(line);

    while(tokenizer >> line)
    {
      std::swap(line.back(), line.front());

      std::cout << line << ' ';
    }
  }
}
