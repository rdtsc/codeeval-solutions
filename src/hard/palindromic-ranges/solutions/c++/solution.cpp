#include <bitset>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <type_traits>
#include <utility>

template<typename T> static inline bool isPalindrome(const T n)
{
  static_assert(std::is_unsigned<T>::value, "Expecting an unsigned type.");

  T reversed = 0;

  for(T i = n; i; i /= 10)
  {
    reversed = (reversed * 10) + (i % 10);
  }

  return (reversed == n);
}

template<typename T> static inline bool isInteresting(const T lowerBound,
                                                      const T upperBound)
{
  static constexpr std::size_t memoThreshold = 1000;
  static std::bitset<memoThreshold> memo, cache;

  std::size_t palindromeCount = 0;

  // Count the number of palindromes in current range.
  for(T i = lowerBound; i <= upperBound; ++i)
  {
    if(i < memoThreshold)
    {
      if(!memo[i])
      {
        memo[i] = true;
        cache[i] = ::isPalindrome(i);
      }

      palindromeCount += cache[i];
    }

    else palindromeCount += ::isPalindrome(i);
  }

  // A range is considered to be "interesting" if it contains
  // an even number of palindromic terms.
  return !(palindromeCount & 1);
}

using Range = std::pair<unsigned, unsigned>;

static std::istream& operator>>(std::istream& inputStream, Range& range)
{
  return inputStream >> range.first >> range.second;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(Range range; inputStream >> range;)
  {
    std::size_t interestingRangeCount = 0;

    for(auto i = range.first; i <= range.second; ++i)
    {
      for(auto j = i; j <= range.second; ++j)
      {
        interestingRangeCount += ::isInteresting(i, j);
      }
    }

    std::cout << interestingRangeCount << '\n';
  }
}
