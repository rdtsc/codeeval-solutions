#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(std::string line; std::getline(inputStream, line);)
  {
    using value_type = unsigned;

    value_type probes[26] = {};
    static constexpr auto probeCount = sizeof probes / sizeof *probes;

    // Store letter frequencies, disregarding case.
    for(const auto c : line) if(::isalpha(c)) ++probes[::tolower(c) - 'a'];

    std::sort(probes, std::end(probes), std::greater<value_type>());

    value_type beautyScore = 0,          // Total beauty score of the string.
               beautyIndex = probeCount; // Maximum letter score multiplier.

    for(const auto letterCount : probes)
    {
      beautyScore += (beautyIndex--) * letterCount;
    }

    std::cout << beautyScore << '\n';
  }
}
