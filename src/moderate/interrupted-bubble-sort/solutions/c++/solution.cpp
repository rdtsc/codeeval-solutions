#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <locale>
#include <vector>

template<class Iterator>
static void bubbleSort(const Iterator first, const Iterator last)
{
  const auto second = std::next(first), tail = std::prev(last);

  for(auto i = second, j = first; j != tail;
      i = std::next(i), j = std::next(j))
  {
    if(*i < *j) std::iter_swap(i, j);
  }
}

template<class Iterator, typename T>
static void interruptedBubbleSort(const Iterator first,
                                  const Iterator last,
                                  const T maxIterations)
{
  const T sequenceLength = std::distance(first, last),
          iterationLimit = std::min(sequenceLength, maxIterations);

  for(T i = 0; i < iterationLimit; ++i)
  {
    ::bubbleSort(first, last);
  }
}

template<class Container>
static bool getValues(std::istream& inputStream, Container& values)
{
  values.clear();

  if(!inputStream) return false;

  for(typename Container::value_type value{}; inputStream >> value;)
  {
    values.push_back(value);

    switch(inputStream.peek()) case '\n': case EOF: return true;
  }

  return false;
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

  using container_type = std::vector<std::uint_fast64_t>;

  for(container_type values; getValues(inputStream, values); std::cout << '\n')
  {
    // The requested number of partial bubble sort passes is located at the
    // very end of the container.
    const auto begin = values.begin(), end = values.end() - 1;

    ::interruptedBubbleSort(begin, end, values.back());

    using output_iterator = std::ostream_iterator<container_type::value_type>;

    std::copy(begin, end, output_iterator(std::cout, " "));
  }
}
