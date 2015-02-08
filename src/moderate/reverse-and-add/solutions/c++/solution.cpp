#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>

template<typename T> static T reverseDigits(T n)
{
  T result = 0;

  for(;n; n /= 10) result = (result * 10) + (n % 10);

  return result;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(std::uint_fast64_t n = 0; inputStream >> n;)
  {
    decltype(n) m = n + ::reverseDigits(n), stepCount = 1;

    for(;m != ::reverseDigits(m); ++stepCount) m += ::reverseDigits(m);

    std::cout << stepCount << ' ' << m << '\n';
  }
}
