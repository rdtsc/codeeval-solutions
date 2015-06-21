#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>

template<typename T>
static bool extractFrom(std::istream& inputStream, T& value)
{
  if(inputStream.peek() == ',') inputStream.ignore();

  return (inputStream >> value);
}

template<typename T, typename... Values>
static bool extractFrom(std::istream& inputStream, T& value, Values&... values)
{
  return ::extractFrom(inputStream, value) &&
         ::extractFrom(inputStream, values...);
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
  using count_type = unsigned char;

  value_type n = 0, k = 0, a = 0, b = 0, c = 0, r = 0;

  while(::extractFrom(inputStream, n, k, a, b, c, r))
  {
    std::array<value_type, (1 << 10)> values{0};
    std::array<count_type, (1 << 16)> counts{0};

    const auto countsBegin = counts.cbegin(),
               countsEnd   = counts.cend();

    // Reduce search space. Target array is cyclic with T <- (K + 1).
    n = (k + (n - k) % (k + 1));

    // Inject known values.
    {
      ++counts[values[0] = a];

      for(value_type i = 1; i < k; ++i)
      {
        ++counts[values[i] = (b * values[i - 1] + c) % r];
      }
    }

    // Inject unknown values.
    for(value_type i = k; i < n; ++i)
    {
      --counts[values[i - k]];

      const auto slot = std::find_if(countsBegin, countsEnd,
        [](const value_type value) {return !value;});

      ++counts[values[i] = std::distance(countsBegin, slot)];
    }

    std::cout << values[--n] << '\n';
  }
}
