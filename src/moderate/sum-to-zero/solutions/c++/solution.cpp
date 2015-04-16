#include <cassert>
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

  std::vector<int> terms;

  for(decltype(terms)::value_type n = 0; inputStream >> n;)
  {
    terms.push_back(n);

    switch(inputStream.peek()) case '\n': case EOF:
    {
      const auto termCount = terms.size();

      assert(termCount >= 4);

      unsigned result = 0;

      // Only dealing with 4 terms at a time, so this should be OK.
      for(decltype(terms)::size_type a = 0 + 0; a < termCount - 3; ++a)
      for(decltype(terms)::size_type b = a + 1; b < termCount - 2; ++b)
      for(decltype(terms)::size_type c = b + 1; c < termCount - 1; ++c)
      for(decltype(terms)::size_type d = c + 1; d < termCount - 0; ++d)
      {
        result += !(terms[a] + terms[b] + terms[c] + terms[d]);
      }

      std::cout << result << '\n';

      terms.clear();
    }
  }
}
