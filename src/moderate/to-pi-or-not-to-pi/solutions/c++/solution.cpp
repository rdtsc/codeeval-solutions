#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  constexpr char digits[] =
  {
    // perl -Mbignum=bpi -e 'print bpi(5000)'

    #include "digits.dat"
  };

  constexpr auto digitCount = (sizeof digits - 1);

  for(std::size_t n = 0; inputStream >> n;)
  {
    assert(n && (n <= digitCount));
    std::cout << digits[--n] << '\n';
  }
}
