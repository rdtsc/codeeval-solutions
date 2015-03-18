#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

// Brute-force search. Fast enough for this problem.
template<typename T> static inline bool isPrime(const T n)
{
  if(n <= 1)        return false; // 0 and 1 -> n/a.
  else if(n < 4)    return true;  // 2 and 3 -> prime.
  else if(!(n & 1)) return false; // Even numbers -> composite.
  else if(n < 9)    return true;  // 5 and 7 -> prime.
  else if(!(n % 3)) return false; // Multiples of 3 -> composite.

  const T limit = std::sqrt(n);

  // Primes >3 are of the form (6n +/- 1).
  for(T i = 5; i <= limit; i += 6)
  {
    if(!(n % i) || !(n % (i + 2))) return false;
  }

  return true;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  using value_type = unsigned;
  using range_type = std::pair<value_type, value_type>;

  range_type globalBounds(std::numeric_limits<value_type>::max(), value_type());

  // Fetch all the range pairs and get the min/max values in the input.
  auto ranges = [&]
  {
    std::vector<range_type> ranges;

    for(range_type range; inputStream; ranges.push_back(range))
    {
      if(!(inputStream >> range.first)) break;

      inputStream.ignore();
      inputStream >> range.second;

      assert(range.first < range.second);

      globalBounds.first  = std::min(globalBounds.first,  range.first);
      globalBounds.second = std::max(globalBounds.second, range.second);
    }

    return ranges;
  }();

  assert(globalBounds.second && (globalBounds.second > globalBounds.first));

  // Brute force. A sieve makes more sense, especially for large ranges, but it
  // looks like we'll only be dealing with a bunch of overlapping ranges all of
  // which seem to max out at ~500.
  auto primes = [&]
  {
    const std::size_t termCount =
      globalBounds.second - globalBounds.first + 1;

    std::vector<bool> primes(termCount);

    for(value_type i = 0; i < termCount; ++i)
    {
      if(::isPrime(globalBounds.first + i)) primes[i] = true;
    }

    return primes;
  }();

  const auto primesBegin = primes.cbegin();

  // Input range playback.
  for(const auto& range : ranges)
  {
    // Rage bounds are inclusive.
    const auto begin = primesBegin + (range.first  - globalBounds.first),
               end   = primesBegin + (range.second - globalBounds.first) + 1;

    std::cout << std::count(begin, end, true) << '\n';
  }
}
