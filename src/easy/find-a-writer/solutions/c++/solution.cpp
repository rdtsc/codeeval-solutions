#include <cassert>
#include <cstddef>
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

  for(std::string line; std::getline(inputStream, line); std::cout << '\n')
  {
    // Indices start after the pipe separator.
    const auto breakpoint = line.find('|');

    assert(breakpoint != std::string::npos);

    const std::string alphabet = line.substr(0, breakpoint);
    std::istringstream keys(line.substr(breakpoint + 1));

    for(std::size_t key = 0; keys >> key;)
    {
      // Supplied indices are 1-based.
      assert(key && key <= alphabet.size());

      std::cout << alphabet[key - 1];
    }
  }
}
