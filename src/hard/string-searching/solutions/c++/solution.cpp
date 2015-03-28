#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

// Non-STDC.
#include <fnmatch.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::boolalpha(std::cout);

  for(std::string line; std::getline(inputStream, line); std::cout << '\n')
  {
    const auto delimiterPos = line.rfind(',');

    assert(delimiterPos != std::string::npos);

    // Enclose raw pattern with asterisks.
    const std::string pattern = '*' + line.substr(delimiterPos + 1) + '*';

    // Isolate the haystack from the needle.
    line.erase(delimiterPos);

    // Non-STDC. May have to implement a lightweight DIY equivalent depending
    // on the score this yields on CodeEval.
    std::cout << (::fnmatch(pattern.c_str(), line.c_str(), 0) != FNM_NOMATCH);
  }
}
