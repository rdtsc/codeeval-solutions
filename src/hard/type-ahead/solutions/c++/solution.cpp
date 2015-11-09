#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <string>
#include <vector>

struct Query
{
  Query() : length(), key() {}

  friend std::istream& operator>>(std::istream& inputStream, Query& query)
  {
    char ignore = '\0';

    if(inputStream >> query.length >> ignore)
    {
      std::getline(inputStream, query.key);
    }

    return inputStream;
  }

  std::size_t length;
  std::string key;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  static const char* const words[] =
  {
    #include "words.dat"
  };

  // As per the problem statement.
  std::cout << std::fixed << std::setprecision(3);

  using index_type = std::map<std::string, std::map<const char*, std::size_t>>;

  std::map<std::size_t, index_type> indexCache;

  for(Query query; inputStream >> query;)
  {
    const auto& index = [&]
    {
      if(indexCache.find(query.length) != indexCache.cend())
      {
        return indexCache[query.length];
      }

      index_type index;

      static constexpr auto wordCount = (sizeof words / sizeof *words);

      /*
        For a bigram, foo bar
                      ^^^ ^^^
                       |   |
                      key val

        For a trigram, foo bar baz
                       ^^^^^^^ ^^^
                          |     |
                         key   val

        etc.
      */

      // Record the words (and their frequency) that appear after n-grams of
      // the supplied width.
      for(auto i = decltype(wordCount){}; i <= (wordCount - query.length); ++i)
      {
        std::string key;

        for(auto j = i; j < (i + query.length - 1); ++j)
        {
          key += words[j];
          key += ' ';
        }

        // Kill the trailing space character.
        key.pop_back();

        ++index[key][words[i + query.length - 1]];
      }

      indexCache.emplace(query.length, index);

      return indexCache[query.length];
    }();

    // Sorted predictions (by probability or alphabetically) based on the
    // supplied n-gram.
    const auto predictions = [&]
    {
      std::vector<std::pair<const char*, float>> result;

      // Total number of words found.
      const auto wordCount = [&]
      {
        std::size_t result = 0;

        for(const auto& record : index.at(query.key))
        {
          result += record.second;
        }

        return static_cast<float>(result);
      }();

      for(const auto& record : index.at(query.key))
      {
        const auto probability = (record.second / wordCount);

        result.emplace_back(record.first, probability);
      }

      using result_value_type = decltype(result)::value_type;

      std::sort(result.begin(), result.end(), [](const result_value_type& lhs,
                                                 const result_value_type& rhs)
      {
        static constexpr auto epsilon = std::numeric_limits<float>::epsilon();

        // Alpha order breaks the tie.
        if(std::fabs(lhs.second - rhs.second) < epsilon)
        {
          return (std::strcmp(lhs.first, rhs.first) < 0);
        }

        return (lhs.second > rhs.second);
      });

      return result;
    }();

    const auto predictionsBegin = predictions.cbegin(),
               predictionsEnd   = predictions.cend();

    std::cout << predictionsBegin->first << ',' << predictionsBegin->second;

    for(auto i = std::next(predictionsBegin); i != predictionsEnd; ++i)
    {
      std::cout << ';' << i->first << ',' << i->second;
    }

    std::cout << '\n';
  }
}
