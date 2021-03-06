#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>
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

  using value_type = unsigned;

  static constexpr auto pristine = std::numeric_limits<value_type>::max();

  for(value_type n = 0, nPrevious = pristine; inputStream >> n;)
  {
    if(n != nPrevious)
    {
      if(nPrevious != pristine) std::cout << ',';

      std::cout << (nPrevious = n);
    }

    switch(inputStream.peek()) case '\n': case EOF:
    {
      std::cout << '\n';
      nPrevious = pristine;
    }
  }
}
