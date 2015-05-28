#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <locale>
#include <string>
#include <type_traits>
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

  for(std::string seq, sub; inputStream >> seq >> sub;)
  {
    const std::size_t count = [&]
    {
      static std::vector<std::decay<decltype(count)>::type> count;

      count.clear();
      count.resize(sub.size() + 1);

      // Seed.
      count.back() = 1;

      const auto seqBegin = seq.crbegin(),
                 seqEnd   = seq.crend();

      const auto subBegin = sub.cbegin(),
                 subEnd   = sub.cend();

      // Fill the result table, RTL.
      for(auto i = seqBegin; i != seqEnd; ++i)
      for(auto j = subBegin; j != subEnd; ++j)
      {
        // Tally match.
        if(*i == *j)
        {
          const auto index = std::distance(subBegin, j);

          count[index] += count[index + 1];
        }
      }

      return count.front();
    }();

    std::cout << count << '\n';
  }
}
