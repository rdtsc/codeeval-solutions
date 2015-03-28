#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

struct Word
{
  Word() : index() {}

  std::string data;
  std::size_t index;
};

static std::ostream& operator<<(std::ostream& outputStream, const Word& word)
{
  return outputStream << word.data;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::vector<Word> words;
  std::istringstream tokenizer;

  for(std::string line; std::getline(inputStream, line); std::cout << '\n')
  {
     words.clear();

    {
      const auto delimiter = line.find(';');

      assert(delimiter != std::string::npos);

      const auto indices = line.substr(delimiter + 1);

      tokenizer.clear();
      line.erase(delimiter);
      tokenizer.str(line);

      for(Word word; tokenizer >> word.data; words.push_back(word));

      tokenizer.clear();
      tokenizer.str(indices);

      /*
        The last word in the sequence is always missing a supplied index in
        the RHS chunk. For example, the words "Foo Bar Baz" may be transformed
        as follows:

                Source             Scrambled               Result
                ------             ---------               ------
            "Foo Bar Baz"   ->   "Baz Foo Bar"   ->   "Baz Foo Bar;3 1"
              1   2   3            3   1   2            3   1   x
                                                                |
                                                            *omitted*

        The index for the last word ("Bar") is always omitted in the input. We
        can reconstruct this index by:

          1. Summing all word indices (in effect, this is just the word count);
          2. Computing the sum of the arithmetic range [1, word count];
          3. Subtracting the latter from the former.
      */

      {
        std::size_t indexSum = 0;
        auto word = words.begin();

        for(std::size_t index = 0; tokenizer >> index; ++word)
        {
          indexSum += index;

          assert(word != words.end());

          word->index = (index - 1);
        }

        const std::size_t expectedIndexSum =
          (words.size() * (1 + words.size())) / 2;

        words.back().index = expectedIndexSum - indexSum - 1;
      }

      std::sort(words.begin(), words.end(), [](const Word& lhs,
                                               const Word& rhs)
      {
        return (lhs.index < rhs.index);
      });
    }

    std::copy(words.cbegin(), words.cend(),
      std::ostream_iterator<Word>(std::cout, " "));
  }
}
