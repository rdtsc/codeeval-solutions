#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <locale>
#include <vector>

template<class Iterator, typename T>
static void interruptedCocktailSort(const Iterator first,
                                    const Iterator last,
                                    const T iterations)
{
  const auto tail = std::prev(last);

  for(auto i = decltype(iterations){}; i < iterations; ++i)
  {
    for(auto j = first; j != tail; ++j)
    {
      if(*j > *(j + 1)) std::iter_swap(j, (j + 1));
    }

    for(auto j = tail; j != first; --j)
    {
      if(*j < *(j - 1)) std::iter_swap(j, (j - 1));
    }
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

  for(value_type value = 0; inputStream >> value;)
  {
    values.emplace_back(value);

    switch(inputStream.peek()) case '\n': case EOF:
    {
      assert(values.size() >= 2);

      const auto iterationCount = values.back();

      // As per the problem statement.
      assert((iterationCount >= 1) && (iterationCount <= 30));

      values.pop_back();

      ::interruptedCocktailSort(values.begin(), values.end(), iterationCount);

      for(const auto value : values) std::cout << value << ' ';

      std::cout << '\n';

      values.clear();
    }
  }
}
