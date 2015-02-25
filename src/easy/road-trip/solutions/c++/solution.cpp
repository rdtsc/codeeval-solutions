#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
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

  // Add printable characters (sans digits) to the list of stream delimiters.
  inputStream.imbue(std::locale(inputStream.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    for(char c = ' '; c <= '/'; ++c) categories[c] |= facet::space;
    for(char c = ':'; c <= '~'; ++c) categories[c] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  // All distances from the origin.
  std::vector<unsigned> distances;

  distances.reserve(600);

  for(decltype(distances)::value_type distance = 0; inputStream >> distance;)
  {
    distances.push_back(distance);

    // End of input line.
    switch(inputStream.ignore().peek()) case '\n': case EOF:
    {
      std::sort(distances.begin(), distances.end());
      std::cout << distances.front();

      const auto distanceCount = distances.size();

      for(auto i = decltype(distanceCount){1}; i < distanceCount; ++i)
      {
        std::cout << ',' << distances[i] - distances[i - 1];
      }

      distances.clear();
      std::cout << '\n';
    }
  }
}
