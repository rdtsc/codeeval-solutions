#include <algorithm>
#include <cassert>
#include <fstream>
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

  // Add commas and semicolons to the list of stream delimiters.
  inputStream.imbue(std::locale(inputStream.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    categories[','] = categories[';'] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  std::vector<int> elements;

  for(decltype(elements)::value_type n = 0; inputStream >> n;)
  {
    elements.push_back(n);

    switch(inputStream.peek()) case '\n': case EOF:
    {
      // We are to reverse the list K elements at a time.
      const auto k = elements.back();

      elements.pop_back();

      const auto groupCount = elements.size() / k;

      auto begin = elements.begin(),
           end   = elements.end();

      for(auto i = decltype(groupCount){}; i < groupCount; ++i)
      {
        std::reverse(std::next(begin, i * k),
                     std::next(begin, i * k + k));
      }

      for(std::cout << *(begin++); begin != end; ++begin)
      {
        std::cout << ',' << *begin;
      }

      std::cout << '\n';
      elements.clear();
    }
  }
}
