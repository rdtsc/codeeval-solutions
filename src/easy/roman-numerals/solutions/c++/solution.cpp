#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>

template<typename T> struct Numeral
{
  using value_type = T;

  Numeral(const std::string& symbol, const T value) : symbol(symbol),
                                                      value(value)
  {
  }

  operator T() const
  {
    return this->value;
  }

  std::string symbol;
  T value;
};

template<typename T> static std::ostream& operator<<(std::ostream& outputStream,
                                                     const Numeral<T>& numeral)
{
  assert(numeral && !numeral.symbol.empty());

  return outputStream << numeral.symbol;
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

  static const Numeral<value_type> numerals[] =
  {
    #define MAP(Symbol, Value) Numeral<value_type>(#Symbol, Value)

    MAP(I,   1), MAP(IV,   4), MAP(V,   5), MAP(IX,   9),
    MAP(X,  10), MAP(XL,  40), MAP(L,  50), MAP(XC,  90),
    MAP(C, 100), MAP(CD, 400), MAP(D, 500), MAP(CM, 900),
    MAP(M, 1e3)

    #undef MAP
  };

  for(value_type n = 0; inputStream >> n; std::cout << '\n')
  {
    // Find the highest numeral that is <= N and subtract it from N,
    // repeating until N reaches zero.
    for(std::size_t i = sizeof numerals / sizeof *numerals; n;)
    for(std::size_t j = i; j--;)
    {
      const auto& numeral = numerals[j];

      if(n >= numeral)
      {
        std::cout << numeral;

        n -= numeral;
        i = j + 1;

        break;
      }
    }
  }
}
