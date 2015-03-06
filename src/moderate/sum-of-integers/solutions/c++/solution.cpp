#include <algorithm>
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

  std::vector<int> numbers;

  for(decltype(numbers)::value_type n = 0; inputStream >> n;)
  {
    numbers.push_back(n);

    switch(inputStream.peek()) case '\n': case EOF:
    {
      auto maxSum = numbers.front();
      auto localMax = maxSum;

      const auto numbersEnd = numbers.cend();

      for(auto i = numbers.begin() + 1; i != numbersEnd; ++i)
      {
        localMax = std::max(*i, localMax + *i);
        maxSum   = std::max(maxSum, localMax);
      }

      std::cout << maxSum << '\n';
      numbers.clear();
    }
  }
}
