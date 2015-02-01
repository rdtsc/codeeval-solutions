#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

static void dumpDigitGroup(const std::uint_fast32_t group,
                           std::ostream& outputStream = std::cout)
{
  static const char* const alphabet[] =
  {
    // [0, 9] += 1
    "Zero", "One", "Two", "Three", "Four",
    "Five", "Six", "Seven", "Eight", "Nine",

    // [10, 19] += 1
    "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen",
    "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen",

    // [20, 90] += 10
    "Twenty", "Thirty", "Forty", "Fifty",
    "Sixty", "Seventy", "Eighty", "Ninety"
  };

  // Digit groups consist of 3 digits in the form of: ABC
  const std::size_t a = group / 100, bc = group % 100;

  if(a) outputStream << alphabet[a] << "Hundred";

  if(bc && bc <= 19) outputStream << alphabet[bc];

  else if(bc >= 20)
  {
    outputStream << alphabet[18 + (bc / 10)];

    const auto c = bc % 10;

    if(c) outputStream << alphabet[c];
  }
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
    // Split N into 3-digit groups.
    const auto digitGroups = [n]
    {
      std::vector<std::uint_fast32_t> result;

      for(auto m = n; m; m /= 1000) result.push_back(m % 1000);

      return result;
    }();

    static const char* const bases[] =
    {
      "", "Thousand", "Million", "Billion"
    };

    // Start from N's highest base.
    std::size_t baseIndex = ::log10(n) / 3;

    // Print the groups in reverse, along with their base.
    std::for_each(digitGroups.rbegin(), digitGroups.rend(),
    [&baseIndex](const std::uint_fast32_t group)
    {
      ::dumpDigitGroup(group);

      if(group) std::cout << bases[baseIndex];

      --baseIndex;
    });

    std::cout << "Dollars\n"; // Always plural.
  }
}
