#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>

template<typename T> static void dump(const std::map<T, std::vector<T>>& data,
                                      std::ostream& outputStream = std::cout)
{
  assert(!data.empty());

  for(const auto& row : data)
  {
    assert(!row.second.empty());

    const auto rhsBegin = row.second.cbegin(),
               rhsEnd   = row.second.cend();

    outputStream << row.first << ':' << *rhsBegin;

    for(auto i = std::next(rhsBegin); i != rhsEnd; ++i)
    {
      outputStream << ',' << *i;
    }

    outputStream << "; ";
  }

  outputStream << '\n';
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  using value_type = unsigned;

  std::map<value_type, std::vector<value_type>> results;

  for(value_type team = 0, country = 1; inputStream >> team;)
  {
    results[team].emplace_back(country);

    while(inputStream.peek() == ' ') inputStream.ignore();

    switch(inputStream.peek())
    {
      // New country.
      case '|':
      {
        ++country;
        inputStream.ignore();
      }
      break;

      // Test case EOL.
      case '\n': case EOF:
      {
        ::dump(results);
        results.clear();
        country = 1;
      }
      break;
    }
  }
}
