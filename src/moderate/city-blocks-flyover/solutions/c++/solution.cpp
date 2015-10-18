#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <locale>
#include <vector>

template<typename T>
static unsigned getFlightPathIntersections(const std::vector<T>& streets,
                                           const std::vector<T>& avenues)
{
  assert(!streets.empty() && !avenues.empty());

  unsigned result = 0;

  const float slope = [&]
  {
    const auto x = streets.back(),
               y = avenues.back();

    assert(x == *std::max_element(streets.cbegin(), streets.cend()));
    assert(y == *std::max_element(avenues.cbegin(), avenues.cend()));

    return (static_cast<float>(y) / x);
  }();

  const auto streetsBegin = streets.cbegin(),
             avenuesBegin = avenues.cbegin();

  const auto streetsEnd = std::prev(streets.cend()),
             avenuesEnd = std::prev(avenues.cend());

  for(auto i = streetsBegin; i != streetsEnd; ++i)
  {
    const auto thisStreet = (*i * slope),
               nextStreet = (*std::next(i) * slope);

    for(auto j = avenuesBegin; j != avenuesEnd; ++j)
    {
      const auto thisAvenue = *j,
                 nextAvenue = *std::next(j);

      if((thisStreet < nextAvenue) && (nextStreet > thisAvenue)) ++result;
    }
  }

  return result;
}

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

    categories[','] = categories['('] = categories[')'] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  using value_type = unsigned;

  for(std::vector<value_type> streets, avenues; inputStream;)
  {
    streets.clear();
    avenues.clear();

    // Fetch streets.
    for(value_type value = 0;
        inputStream.peek() != ')' && inputStream >> value;)
    {
      streets.push_back(value);
    }

    if(inputStream.peek() == EOF) break;

    // Kill trailing token.
    {
      assert(inputStream.peek() == ')');

      inputStream.ignore();
    }

    // Fetch avenues.
    for(value_type value = 0;
        inputStream.peek() != ')' && inputStream >> value;)
    {
      avenues.push_back(value);
    }

    // Kill trailing token.
    {
      assert(inputStream.peek() == ')');

      inputStream.ignore();
    }

    // Values appear to be pre-sorted in the input.
    assert(std::is_sorted(streets.cbegin(), streets.cend()));
    assert(std::is_sorted(avenues.cbegin(), avenues.cend()));

    std::cout << ::getFlightPathIntersections(streets, avenues) << '\n';
  }
}
