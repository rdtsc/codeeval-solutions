#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <type_traits>
#include <vector>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  const auto explode = [](const std::string& source,
                          std::vector<std::string>& destination)
  {
    static std::istringstream tokenizer;

    tokenizer.clear();
    tokenizer.str(source);
    destination.clear();

    for(static std::string word; tokenizer >> word;)
    {
      destination.push_back(word);
    }
  };

  const auto dump = [](const std::vector<std::string>& source)
  {
    using output_iterator =
      std::ostream_iterator<std::decay<decltype(source)>::type::value_type>;

    std::copy(source.cbegin(), source.cend(), output_iterator(std::cout, " "));
  };

  std::vector<std::string> input, filter;

  for(std::string lhs, rhs;
      std::getline(inputStream, lhs, ';') && std::getline(inputStream, rhs);
      std::cout << '\n')
  {
    explode(lhs, input);

    // Supplied filter is empty. All words should be censored.
    if(rhs.empty())
    {
      for(auto& word : input) std::fill(word.begin(), word.end(), '_');

      dump(input);

      continue;
    }

    explode(rhs, filter);

    const bool isFullyCensored = [&]
    {
      static constexpr auto notFound   = std::string::npos,
                            replaceAll = std::string::npos;

      auto replacement = filter.cbegin();
      const auto filterEnd = filter.cend();

      for(auto& word : input)
      {
        const auto cursor = (replacement != filterEnd ?
                             word.find(*replacement) :
                             notFound);

        // Match. Partial or otherwise.
        if(cursor != notFound)
        {
          // Mask leading characters.
          word.replace(0, cursor, cursor, '_');

          const auto wordSize = word.size();

          const auto replacementSize = replacement->size(),
                     replacementStart = (cursor + replacementSize),
                     replacementLength = (wordSize - replacementSize - cursor);

          // Mask trailing characters.
          word.replace(replacementStart, replaceAll, replacementLength, '_');

          ++replacement;
        }

        // No match. Completely censor this word.
        else std::fill(word.begin(), word.end(), '_');
      }

      return (replacement == filterEnd);
    }();

    if(!isFullyCensored) std::cout << "I cannot fix history";

    else dump(input);
  }
}
