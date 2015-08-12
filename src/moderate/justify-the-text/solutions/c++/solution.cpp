#include <cassert>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

static void dumpWords(const std::vector<std::string>& words,
                      const std::size_t spaceCount = 1,
                      std::size_t padding = 0,
                      std::ostream& outputStream = std::cout)
{
  const auto wordsBegin = words.cbegin(),
             wordsEnd   = words.cend();

  assert(wordsBegin != wordsEnd);

  outputStream << *wordsBegin;

  for(auto i = std::next(wordsBegin); i != wordsEnd; ++i)
  {
    const bool shouldInjectPadding = !!(padding ? padding-- : false);
    const auto totalWidth = (spaceCount + shouldInjectPadding);

    std::cout << std::setw(totalWidth) << ' ' << *i;
  }

  outputStream << '\n';
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::vector<std::string> words;
  std::istringstream tokenizer;

  static constexpr std::size_t rulerColumn = 80;

  for(std::string line; std::getline(inputStream, line);)
  {
    words.clear();
    tokenizer.clear();
    tokenizer.str(line);

    for(std::size_t lineWidth = 0; tokenizer >> line;)
    {
      const auto wordWidth = line.size();

      /*
        |foo bar baz bax     |
        |foo   bar   baz  bax|
            ^^^   ^^^   ^^
      */

      if((wordWidth + lineWidth) > rulerColumn)
      {
        const auto wordCount  = (words.size() - 1),
                   margin     = (rulerColumn + 1 - lineWidth),
                   padding    = (margin % wordCount),
                   spaceCount = (margin / wordCount + 1);

        ::dumpWords(words, spaceCount, padding);

        words.clear();
        lineWidth = 0;
      }

      words.push_back(line);
      lineWidth += (wordWidth + 1);
    }

    ::dumpWords(words);
  }
}
