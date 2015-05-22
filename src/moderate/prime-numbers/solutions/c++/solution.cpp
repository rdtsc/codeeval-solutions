#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  using value_type = unsigned;
  using input_iterator = std::istream_iterator<value_type>;

  const std::vector<value_type>
    terms{input_iterator(inputStream), input_iterator()};

  assert(!terms.empty());

  const auto maxTerm =
    *std::max_element(terms.cbegin(), terms.cend());

  assert(maxTerm >= 3);

  const auto isPrime = [&](const value_type n)
  {
    assert(n <= maxTerm);

    // Sieve of Eratosthenes.
    static const auto primalityCache = [&]
    {
      // Don't tase me, bro!
      std::vector<bool> result(maxTerm, true);

      const value_type limit = ::sqrt(maxTerm);

      for(auto i = decltype(limit){2}; i <= limit; ++i) if(result[i])
      {
        for(auto j = (i * i); j <= maxTerm; j += i)
        {
          result[j] = false;
        }
      }

      return result;
    }();

    return primalityCache[n];
  };

  // Input playback. This is really expensive; should pre-format the sequence.
  for(const auto term : terms)
  {
    std::cout << '2';

    for(value_type i = 3; i < term; ++i) if(isPrime(i))
    {
      std::cout << ',' << i;
    }

    std::cout << '\n';
  }
}
