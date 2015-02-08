#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>

template<typename CentType = unsigned> class Transaction
{
  private: friend std::istream&
    operator>>(std::istream& inputStream, Transaction& transaction)
  {
    transaction.price = transaction.cash = 0;

    std::string chunk;

    // Everything to the left of the semicolon is part of the purchase price.
    {
      std::getline(inputStream, chunk, ';');

      for(const auto c : chunk) if(::isdigit(c))
      {
        transaction.price = (transaction.price * 10) + (c - '0');
      }

      // Convert whole dollar values (i.e., no fractional part) to cents.
      if(chunk.find('.') == std::string::npos) transaction.price *= 100;
    }

    // Everything to the right of the semicolon is part of the cash figure.
    {
      std::getline(inputStream, chunk);

      for(const auto c : chunk) if(::isdigit(c))
      {
        transaction.cash = (transaction.cash * 10) + (c - '0');
      }

      // Convert whole dollar values (i.e., no fractional part) to cents.
      if(chunk.find('.') == std::string::npos) transaction.cash *= 100;
    }

    return inputStream;
  }

  private: friend std::ostream&
    operator<<(std::ostream& outputStream, const Transaction& transaction)
  {
    static const std::map<CentType, std::string> denominations =
    {
      {1, "PENNY"}, {5, "NICKEL"}, {10, "DIME"}, {25, "QUARTER"},
      {50, "HALF DOLLAR"}, {100, "ONE"}, {200, "TWO"}, {500, "FIVE"},
      {1000, "TEN"}, {2000, "TWENTY"}, {5000, "FIFTY"}, {10000, "ONE HUNDRED"}
    };

    static const auto denominationsBegin = denominations.rbegin(),
                      denominationsEnd   = denominations.rend();

    if(transaction.price  > transaction.cash) return outputStream << "ERROR\n";
    if(transaction.price == transaction.cash) return outputStream << "ZERO\n";

    CentType change = transaction.cash - transaction.price;

    for(auto i = denominationsBegin; change;)
    {
      if(i != denominationsEnd && i->first > change) ++i;

      else
      {
        change -= i->first;

        outputStream << i->second;

        if(change) outputStream << ',';
      }
    }

    return outputStream << '\n';
  }

  private: CentType price, cash;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  using input_iterator  = std::istream_iterator<Transaction<>>;
  using output_iterator = std::ostream_iterator<Transaction<>>;

  std::copy(input_iterator(inputStream),
            input_iterator(),
            output_iterator(std::cout));
}
