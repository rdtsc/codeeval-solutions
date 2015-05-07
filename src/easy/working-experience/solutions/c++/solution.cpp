#include <algorithm>
#include <bitset>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>
#include <unordered_map>
#include <vector>

template<unsigned EpochYear> struct Date
{
  Date() : year(), month() {}

  unsigned getEpochOffset() const
  {
    return ((this->year - EpochYear) * 12 + this->month);
  }

  friend std::istream& operator>>(std::istream& inputStream, Date& date)
  {
    date = {};

    std::string month;

    if(!(inputStream >> month >> date.year)) return inputStream;

    std::transform(month.begin(), month.end(), month.begin(), ::tolower);

    static const std::unordered_map<std::string, unsigned> calendar =
    {
      {"jan", 1}, {"feb",  2}, {"mar",  3}, {"apr",  4},
      {"may", 5}, {"jun",  6}, {"jul",  7}, {"aug",  8},
      {"sep", 9}, {"oct", 10}, {"nov", 11}, {"dec", 12}
    };

    date.month = calendar.at(month);

    return inputStream;
  }

  unsigned year, month;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::istringstream tokenizer;

  // Add hyphens and semicolons to the list of stream delimiters.
  tokenizer.imbue(std::locale(tokenizer.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    categories['-'] = categories[';'] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  for(std::string line; std::getline(inputStream, line);)
  {
    tokenizer.clear();
    tokenizer.str(line);

    static constexpr unsigned epochYear = 1990;

    // Dates are guaranteed to fall inside of [1990-01-01, 2020-12-31].
    std::bitset<(2020 - epochYear) * 12> employedMonths;

    for(Date<epochYear> lhs, rhs; tokenizer >> lhs >> rhs;)
    {
      assert((lhs.year >= epochYear) && (rhs.year <= 2020));

      const std::size_t begin = lhs.getEpochOffset(),
                        end   = rhs.getEpochOffset();

      assert(end >= begin);

      // Record the relevant months as work experience.
      for(auto i = begin; i <= end; ++i) employedMonths.set(i);
    }

    // Total working experience in years.
    std::cout << (employedMonths.count() / 12) << '\n';
  }
}
