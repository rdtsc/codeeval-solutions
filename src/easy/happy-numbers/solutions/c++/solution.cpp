#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

template<typename T> static bool isHappy(const T n)
{
  if(n == 1) return true;

  static auto reduceDigits = [](T n)
  {
    static const unsigned digitSquares[] =
    {
      0, 1, 4, 9, 16, 25, 36, 49, 64, 81
    };

    unsigned result = 0;

    for(;n; n /= 10) result += digitSquares[n % 10];

    return result;
  };

  // Savings of ~3k over a set on CE.
  static std::vector<unsigned> chain;

  chain.clear();

  // We should terminate if we've arrived at one...
  for(auto i = reduceDigits(n); i != 1; i = reduceDigits(i))
  {
    // ...or have encountered a previously repeated term.
    if(std::find(chain.cbegin(), chain.cend(), i) != chain.cend())
    {
      return false;
    }

    chain.push_back(i);
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

  for(std::uint_fast32_t n = 0; inputStream >> n;)
  {
    std::cout << ::isHappy(n) << '\n';
  }
}
