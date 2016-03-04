#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <locale>
#include <string>
#include <vector>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Add pipes to the list of stream delimiters.
  inputStream.imbue(std::locale(inputStream.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    categories['|'] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  std::string chunk;
  std::vector<std::string> cells;

  while(inputStream)
  {
    for(cells.clear(); inputStream >> chunk;)
    {
      cells.emplace_back(chunk);

      const auto peek = inputStream.peek();

      if(peek == '\n' || peek == EOF) break;
    }

    if(cells.empty()) break;

    const auto w = cells.front().size(),
               h = cells.size();

    // As per the problem statement.
    assert((w >= 2) && (w <= 10));
    assert((h >= 2) && (h <= 10));

    // The matrix should be well-formed.
    assert(std::all_of(cells.cbegin(), cells.cend(),
      [&](const std::string& row)
    {
      return (row.size() == w);
    }));

    // Letters are expected to be lowercase.
    assert(std::all_of(cells.cbegin(), cells.cend(),
      [](const std::string& row)
    {
      return std::all_of(row.cbegin(), row.cend(),
        [](const char c) -> bool
      {
        if(::isalpha(c)) return ::islower(c);

        return true;
      });
    }));

    const auto matchCount = [&]
    {
      unsigned result = 0;

      for(std::size_t y = 0; y < (h - 1); ++y)
      for(std::size_t x = 0; x < (w - 1); ++x)
      {
        static constexpr auto histogramCapacity =
          std::numeric_limits<char>::max();

        unsigned char histogram[histogramCapacity] = {};

        /*
                             (0, 0) +--+ (1, 0)
                                    |  |
                             (0, 1) +--+ (1, 1)
        */

        ++histogram[static_cast<unsigned char>(cells[y + 0][x + 0])];
        ++histogram[static_cast<unsigned char>(cells[y + 0][x + 1])];
        ++histogram[static_cast<unsigned char>(cells[y + 1][x + 0])];
        ++histogram[static_cast<unsigned char>(cells[y + 1][x + 1])];

        result += (histogram['c'] == 1) &&
                  (histogram['o'] == 1) &&
                  (histogram['d'] == 1) &&
                  (histogram['e'] == 1);
      }

      return result;
    }();

    std::cout << matchCount << '\n';
  }
}
