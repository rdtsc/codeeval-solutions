#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
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

  static const auto mapping = []
  {
    std::array<unsigned char, 'Z'> result = {0};

    std::iota(&result['2'], &result[':'], 2);

    #define MAP(Symbol, Value) result[*(#Symbol)] = (Value)

    MAP(10,    10);
    MAP(Jack,  11);
    MAP(Queen, 12);
    MAP(King,  13);
    MAP(Ace,   14);

    #undef MAP

    return result;
  }();

  char trumpSuit = '\0';
  std::string lhs, rhs;

  for(;inputStream >> lhs >> rhs >> trumpSuit; std::cout << '\n')
  {
    assert((lhs.size() >= 2) && (lhs.size() <= 3));
    assert((rhs.size() >= 2) && (rhs.size() <= 3));

    const auto lhsShift = ((lhs.back() == trumpSuit) * 3),
               rhsShift = ((rhs.back() == trumpSuit) * 3);

    const unsigned a = (mapping[lhs.front()] << lhsShift),
                   b = (mapping[rhs.front()] << rhsShift);

    if(a == b) std::cout << lhs << ' ' << rhs;

    else std::cout << (a > b ? lhs : rhs);
  }
}
