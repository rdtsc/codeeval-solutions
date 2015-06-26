#include <cassert>
#include <cstddef>
#include <fstream>
#include <iomanip>
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

  std::cout.fill('*');

  for(std::string word, longestWord; inputStream >> word;)
  {
    if(word.size() > longestWord.size()) longestWord = word;

    switch(inputStream.peek()) case '\n': case EOF:
    {
      std::size_t maskWidth = 1;

      for(const char c : longestWord)
      {
        std::cout << std::setw(maskWidth++) << c << ' ';
      }

      std::cout << '\n';
      longestWord.clear();
    }
  }
}
