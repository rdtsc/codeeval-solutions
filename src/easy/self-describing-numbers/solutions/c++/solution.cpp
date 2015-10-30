#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <type_traits>

template<typename T> static bool isSelfDescribing(T n)
{
  static_assert(std::is_integral<T>::value, "T should be an integral type.");
  assert(n > 0);

  char digitOccurrences[10] = {};

  // Build the digit frequency map.
  for(T i = n; i; i /= 10) ++digitOccurrences[i % 10];

  // Iterate through the digits RTL.
  for(T i = ::log10(n); n; n /= 10)
  {
    // Digit position should correspond to the digit count.
    if(digitOccurrences[i--] != n % 10) return false;
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

  for(unsigned n = 0; inputStream >> n;)
  {
    std::cout << ::isSelfDescribing(n) << '\n';
  }
}
