#include <cassert>
#include <deque>
#include <fstream>
#include <iostream>
#include <locale>
#include <vector>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Add semicolons to the list of stream delimiters.
  inputStream.imbue(std::locale(inputStream.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    categories[';'] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  // Daily portfolio gains/losses.
  std::deque<int> deltas;

  for(decltype(deltas)::value_type n = 0; inputStream >> n;)
  {
    deltas.push_back(n);

    switch(inputStream.peek()) case '\n': case EOF:
    {
      // Number of days we're going to be playing the market.
      const decltype(deltas)::size_type period = deltas.front();

      deltas.pop_front();

      const auto deltaCount = deltas.size();

      decltype(n) maxGain = 0, localSum = 0;

      for(auto i = decltype(period){}; i < period; ++i)
      {
        localSum += deltas[i];
      }

      for(auto i = period; i <= deltaCount; ++i)
      {
        if(localSum > maxGain) maxGain = localSum;

        localSum += deltas[i] - deltas[i - period];
      }

      std::cout << maxGain << '\n';
      deltas.clear();
    }
  }
}
