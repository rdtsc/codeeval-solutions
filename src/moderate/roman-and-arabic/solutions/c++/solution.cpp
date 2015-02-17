#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Converts aromatic numbers to decimal.
  auto toDecimal = [](const std::string& literal)
  {
    assert(!literal.empty() && !(literal.size() & 1));

    using value_type = unsigned;

    static const std::map<char, value_type> romanBases =
    {
      #ifdef B
      #error
      #endif

      #define B(Literal, Value) {*(#Literal), (Value)}

      B(I, 1),  B(V, 5),   B(X, 10),
      B(L, 50), B(C, 100), B(D, 500),
      B(M, 1000)

      #undef B
    };

    std::int_fast64_t result = 0;

    value_type previousBase = 1;

    for(auto i = literal.crbegin(), literalEnd = literal.crend();
        i != literalEnd; i += 2)
    {
      const value_type r  = romanBases.at(*i), // Roman base.
                       a  = *(i + 1) - '0',    // Arabic digit.
                       ra = r * a;             // Aromatic pair.

      result = (previousBase > r) ? (result - ra) : (result + ra);

      previousBase = r;
    }

    return result;
  };

  for(std::string literal; inputStream >> literal;)
  {
    std::cout << toDecimal(literal) << '\n';
  }
}
