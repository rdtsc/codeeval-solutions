#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

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

  std::istringstream tokenizer;

  for(std::string pattern, haystack;
      inputStream >> pattern && std::getline(inputStream, haystack);)
  {
    tokenizer.clear();
    tokenizer.str(haystack);

    auto foundMatches = false;
    const auto basicPattern = pattern.c_str();

    while(tokenizer >> haystack)
    {
      // Non-STDC, but compact, runs quickly, and the supported pattern
      // lexicon fits like a glove.
      if(::fnmatch(basicPattern, haystack.c_str(), 0) != FNM_NOMATCH)
      {
        foundMatches = true;
        std::cout << haystack << ' ';
      }
    }

    if(!foundMatches) std::cout << '-';

    std::cout << '\n';
  }
}
