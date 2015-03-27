#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <sstream>
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

  std::vector<value_type> addresses;
  std::istringstream tokenizer;

  for(std::string line; std::getline(inputStream, line);)
  {
    addresses.clear();
    tokenizer.clear();
    tokenizer.str(line);

    static constexpr auto ignoreLimit =
      std::numeric_limits<std::streamsize>::max();

    // Discard the address count.
    tokenizer.ignore(ignoreLimit, ' ');

    for(value_type n = 0; tokenizer >> n;) addresses.push_back(n);

    assert(addresses.size() >= 2);

    std::sort(addresses.begin(), addresses.end());

    const auto friendCount = addresses.size();

    const auto ourAddress = [&]
    {
      const auto midpoint =
        std::next(addresses.begin(), friendCount / 2);

      if(friendCount & 1) return (*midpoint);

      return (*(midpoint - 1) + *midpoint) / 2;
    }();

    // Get the total distance from our address to all of our friends' houses.
    const auto totalDistance = [&]
    {
      static const auto distanceAccumulator = [&](const value_type total,
                                                  const value_type value)
      {
        static const auto distance = [](const value_type x,
                                        const value_type y)
        {
          return (x > y) ? (x - y) : (y - x);
        };

        return (total + distance(ourAddress, value));
      };

      return std::accumulate(addresses.cbegin(),
        addresses.cend(), value_type(), distanceAccumulator);
    }();

    std::cout << totalDistance << '\n';
  }
}
