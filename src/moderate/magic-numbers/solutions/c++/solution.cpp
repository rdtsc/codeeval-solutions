#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

template<typename T> static inline bool hasUniqueDigits(T n)
{
  std::uint_fast64_t history = 0;

  for(;n; n /= 10)
  {
    const unsigned char digit = (n % 10);

    #define BIT_SET_AT(Position) (history  & (1 << (Position)))
    #define SET_BIT_AT(Position) (history |= (1 << (Position)))

    if(BIT_SET_AT(digit)) return false;

    SET_BIT_AT(digit);

    #undef SET_BIT_AT
    #undef BIT_SET_AT
  }

  return true;
}

template<typename T> static bool isMagicNumber(const T n)
{
  assert(n >= 1 && n <= 1e4);

  if(!::hasUniqueDigits(n)) return false;

  const auto digitCount = [n]
  {
    std::size_t result = 0;

    for(T i = n; i; i /= 10) ++result;

    return result;
  }();

  unsigned char digits[10] = {};

  assert(digitCount && digitCount <= (sizeof digits / sizeof *digits));

  // Stuff N's digits into the `digits` array.
  {
    T m = n;

    for(auto i = digitCount; i--; m /= 10) digits[i] = (m % 10);
  }

  std::uint_fast64_t uniquenessMask = 1;

  for(std::size_t i = *digits % digitCount; i;)
  {
    #define BIT_SET_AT(Position) (uniquenessMask  & (1 << (Position)))
    #define SET_BIT_AT(Position) (uniquenessMask |= (1 << (Position)))

    // We've been at this index before, therefore not a "magic" number.
    if(BIT_SET_AT(i)) return false;

    // First visit.
    SET_BIT_AT(i);
    i = (digits[i] + i) % digitCount;

    #undef SET_BIT_AT
    #undef BIT_SET_AT
  }

  // All digits should have been visited exactly once for this to be
  // considered a "magic" number.
  return (uniquenessMask == (1 << digitCount) - 1);
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  using value_type = std::uint_least16_t;
  using range_type = std::pair<value_type, value_type>;

  static constexpr auto valueTypeMax = std::numeric_limits<value_type>::max();

  static_assert(valueTypeMax > 1e4, "Underlying type is too narrow.");

  range_type globalBounds(valueTypeMax, value_type());

  // Fetch all the range pairs and get the lowest and highest values.
  const auto ranges = [&]
  {
    std::vector<range_type> ranges;

    for(range_type range; inputStream; ranges.push_back(range))
    {
      if(!(inputStream >> range.first >> range.second)) break;

      assert(range.first >= 1 && (range.first <= range.second));

      globalBounds.first  = std::min(globalBounds.first,  range.first);
      globalBounds.second = std::max(globalBounds.second, range.second);
    }

    return ranges;
  }();

  assert(globalBounds.second && (globalBounds.second >= globalBounds.first));

  std::vector<value_type> magicNumbers, magicIndex(globalBounds.second + 1);

  // There are only 89 "magic" numbers <= 1e4.
  magicNumbers.reserve(90);

  // Pre-compute the result set for increased performance.
  for(value_type i = globalBounds.first; i <= globalBounds.second; ++i)
  {
    if(::isMagicNumber(i))
    {
      magicNumbers.push_back(i);

      assert(static_cast<std::size_t>(i) < magicIndex.size());

      magicIndex[i] = magicNumbers.size();
    }
  }

  // Input range playback.
  for(const auto& range : ranges)
  {
    bool foundResults = false;

    for(auto i = range.first; i <= range.second; ++i) if(magicIndex[i])
    {
      foundResults = true;

      std::cout << magicNumbers[magicIndex[i] - 1] << ' ';
    }

    if(!foundResults) std::cout << "-1";

    std::cout << '\n';
  }
}
