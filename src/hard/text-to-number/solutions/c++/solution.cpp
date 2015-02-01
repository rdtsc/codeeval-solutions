#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <type_traits>
#include <vector>

// Converts a sequence of numbers that are expressed as English words and fall
// within the [1, 1000) interval into integers.
template<typename T, class Sequence> static T numberGroup(const Sequence& words)
{
  assert(!words.empty());

  T result = 0;

  static const auto alphabet = []
  {
    std::map<std::string, T> result;

    static const char* const alphabet[] =
    {
      // [0, 9] += 1
      "zero", "one", "two", "three", "four",
      "five", "six", "seven", "eight", "nine",

      // [10, 19] += 1
      "ten", "eleven", "twelve", "thirteen", "fourteen",
      "fifteen", "sixteen", "seventeen", "eighteen", "nineteen",

      // [20, 90] += 10
      "twenty", "thirty", "forty", "fifty",
      "sixty", "seventy", "eighty", "ninety"
    };

    // Map the string literals to their corresponding integer values.
    for(std::size_t i = 0; i < sizeof alphabet / sizeof *alphabet; ++i)
    {
      result[alphabet[i]] = (i < 20) ? (i) : (20 + (i % 10) * 10);
    }

    return result;
  }();

  static const auto alphabetEnd = alphabet.cend();

  for(const auto& word : words)
  {
    if(word == "hundred") result *= 100;

    else if(alphabet.find(word) != alphabetEnd)
    {
      result += alphabet.at(word);
    }
  }

  return result;
}

// Converts numbers written in English words that fall within the (-1e9, 1e9]
// interval into integers.
template<typename T> static T interpretNumber(const std::string& words)
{
  assert(!words.empty());

  static_assert(std::is_signed<T>::value, "T must be a signed type.");

  static_assert(std::numeric_limits<T>::digits >= 31,
    "T must be at least as wide as a signed 32-bit integer.");

  T result = 0;

  std::istringstream tokenizer(words);
  std::vector<std::string> group;

  for(std::string chunk; tokenizer >> chunk;)
  {
    const bool is1e3 = chunk == "thousand",
               is1e6 = !is1e3 && chunk == "million";

    if(!is1e3 && !is1e6) group.push_back(chunk);

    else
    {
      const T groupValue = ::numberGroup<T>(group) * (is1e3 ? 1e3 :
                                                      is1e6 ? 1e6 : 1);

      result += groupValue;
      group.clear();
    }
  }

  // Don't forget the very last group.
  result += ::numberGroup<T>(group);

  const bool isNegative = !words.compare(0, sizeof "negative" - 1, "negative");

  return (isNegative) ? (-result) : (result);
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(std::string line; std::getline(inputStream, line);)
  {
    std::cout << ::interpretNumber<std::int_fast32_t>(line) << '\n';
  }
}
