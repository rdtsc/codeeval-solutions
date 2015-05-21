#include <algorithm>
#include <cassert>
#include <cstddef>
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

  // Add commas to the list of stream delimiters.
  inputStream.imbue(std::locale(inputStream.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    categories[','] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  std::vector<unsigned> roster;

  for(unsigned n = 0, m = 0; inputStream >> n >> m; std::cout << '\n')
  {
    roster.resize(n);

    std::iota(roster.begin(), roster.end(), 1);

    // 0-based count of people.
    for(std::size_t peopleLeft = n, i = (m - 1); peopleLeft;)
    {
      std::cout << (roster[i] - 1) << ' ';

      // RIP.
      roster[i] = 0;

      if(--peopleLeft) for(auto j = decltype(m){}; j < m;)
      {
        // Rewind to start of sequence.
        if(++i >= n) i = 0;

        // Is this a live person?
        if(roster[i]) ++j;
      }
    }
  }
}
