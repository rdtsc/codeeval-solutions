#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>
#include <vector>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  using value_type = unsigned;

  std::vector<value_type> lhs, rhs;
  std::istringstream tokenizer;

  // Add commas to the list of stream delimiters.
  tokenizer.imbue(std::locale(tokenizer.getloc(), []
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

  const auto getList = [&](std::vector<value_type>& range)
  {
    range.clear();

    for(value_type n = 0; tokenizer >> n; range.push_back(n));

    assert(std::is_sorted(range.cbegin(), range.cend()));

    tokenizer.clear();

    if(tokenizer.peek() == ';') tokenizer.ignore();
  };

  for(std::string line; std::getline(inputStream, line); std::cout << '\n')
  {
    tokenizer.clear();
    tokenizer.str(line);

    getList(lhs);
    getList(rhs);

    const auto lhsSize = lhs.size(),
               rhsSize = rhs.size();

    // Both lists are guaranteed to be sorted.
    if(rhs.front() <= lhs[lhsSize - 1] &&
       lhs.front() <= rhs[rhsSize - 1])
    {
      bool isFirstTerm = true;

      for(const auto left  : lhs)
      for(const auto right : rhs)
      {
        if(left < right) break;

        else if(left == right)
        {
          if(!isFirstTerm) std::cout << ',';
          else isFirstTerm = false;

          std::cout << left;
          break;
        }
      }
    }
  }
}
