#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
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

  using value_type = unsigned;

  std::vector<value_type> values;

  for(value_type n = 0; inputStream >> n;)
  {
    values.push_back(n);

    switch(inputStream.peek()) case '\n': case EOF:
    {
      assert(!values.empty() && !(values.size() & 1));

      const auto lhsBegin = values.begin(),
                 lhsEnd   = lhsBegin + (values.size() / 2),
                 rhsBegin = lhsEnd;

      using output_iterator = std::ostream_iterator<value_type>;
      using multiply = std::multiplies<value_type>;

      std::transform(lhsBegin, lhsEnd, rhsBegin,
        output_iterator(std::cout, " "), multiply());

      std::cout << '\n';
      values.clear();
    }
  }
}
