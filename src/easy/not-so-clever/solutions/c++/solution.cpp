#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <locale>
#include <vector>

template<class Iterator, typename T>
static void interruptedDumbSort(const Iterator first,
                                const Iterator last,
                                const T iterations)
{
  const auto tail = std::prev(last);

  for(auto i = decltype(iterations){}; i < iterations; ++i)
  for(auto j = first; j != tail; ++j)
  {
    if(*j > *(j + 1))
    {
      std::iter_swap(j, (j + 1));
      break;
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
      assert(values.size() >= 3);

      const auto iterationCount = values.back();

      // As per the problem statement.
      assert(iterationCount && (iterationCount <= 8));

      values.pop_back();

      ::interruptedDumbSort(values.begin(), values.end(), iterationCount);

      for(const auto value : values) std::cout << value << ' ';

      std::cout << '\n';

      values.clear();
    }
  }
}
