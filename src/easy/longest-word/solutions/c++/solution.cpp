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

  for(std::string word, longestWord; inputStream >> word;)
  {
    if(word.size() > longestWord.size()) longestWord = word;

    switch(inputStream.peek()) case '\n': case EOF:
    {
      std::cout << longestWord << '\n';
      longestWord.clear();
    }
  }
}
