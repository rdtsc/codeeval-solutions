#include <cassert>
#include <cstdint>
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

  using value_type = std::uint_fast64_t;

  value_type coinLimit  = 0,
             valueLimit = 0;

  std::vector<value_type> denominations;

  while(inputStream >> coinLimit >> valueLimit)
  {
    assert(coinLimit  && (coinLimit  <= 100));
    assert(valueLimit && (valueLimit <= 1e9));

    denominations.clear();

    for(value_type value = 0; inputStream >> value;)
    {
      denominations.emplace_back(value);

      const auto peek = inputStream.peek();

      if((peek == '\n') || (peek == EOF)) break;
    }

    assert(!denominations.empty() && (denominations.size() <= 100));

    value_type result = 0,
               target = 1;

    for(const auto denomination : denominations)
    {
      for(;denomination > target; ++result)
      {
        target = (coinLimit + 1) * target;
      }

      target += (denomination * coinLimit);
    }

    for(;target < valueLimit; ++result)
    {
      target = (coinLimit + 1) * target;
    }

    std::cout << result << '\n';
  }
}
