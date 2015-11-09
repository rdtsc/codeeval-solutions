#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <locale>
#include <sstream>
#include <vector>

class CodeEvalFacet : public std::numpunct<char>
{
  protected: virtual std::string do_truename() const override
  {
    return "Yes";
  }

  protected: virtual std::string do_falsename() const override
  {
    return "No";
  }
};

struct SeatingPreferences
{
  SeatingPreferences() : seatCount()
  {
    // Add the following characters to the list of stream delimiters: ";:,[]"
    this->tokenizer.imbue(std::locale(this->tokenizer.getloc(), []
    {
      using facet = std::ctype<char>;

      const auto classicTableStart = facet::classic_table(),
                 classicTableEnd   = classicTableStart + facet::table_size;

      static std::vector<facet::mask> categories(classicTableStart,
                                                 classicTableEnd);

      for(const auto c : ";:,[]") if(c) categories[c] |= facet::space;

      // Ref-counted upstream.
      return new facet(categories.data());
    }()));
  }

  bool canAccommodate() const
  {
    std::vector<int> seats(this->seatCount, -1);
    std::vector<bool> memo(this->seatCount);

    unsigned seatedCount = 0;

    const std::function<bool(std::size_t)> canSeat = [&](const std::size_t i)
    {
      for(auto j = decltype(this->seatCount){}; j < this->seatCount; ++j)
      {
        if(!memo[j] && this->data[i][j])
        {
          memo[j] = true;

          // Try another.
          if((seats[j] == -1) || canSeat(seats[j]))
          {
            seats[j] = i;
            return true;
          }
        }
      }

      return false;
    };

    const auto personCount = this->data.size();

    for(auto i = decltype(personCount){}; i < personCount; ++i)
    {
      // Reset history for current person.
      std::fill(memo.begin(), memo.end(), false);

      if(canSeat(i)) ++seatedCount;
    }

    return (seatedCount == this->seatCount);
  }

  friend std::istream& operator>>(std::istream& inputStream,
                                  SeatingPreferences& preferences)
  {
    std::string line;

    if(!std::getline(inputStream, line)) return inputStream;

    auto& seatCount = preferences.seatCount;
    auto& tokenizer = preferences.tokenizer;

    preferences.data.clear();

    std::istringstream jar(line);

    // Fetch seat count and (dirty) seating preferences.
    for(jar >> seatCount; std::getline(jar, line, ']');)
    {
      tokenizer.clear();
      tokenizer.str(line);

      std::vector<unsigned> row(seatCount);

      unsigned id = 0;

      // Record the person's preferred seats.
      // Don't really need to retain the person's ID.
      for(tokenizer >> id; tokenizer >> id;)
      {
        row[id - 1] = 1;
      }

      preferences.data.emplace_back(row);
    }

    return inputStream;
  }

  unsigned seatCount;
  std::istringstream tokenizer;
  std::vector<std::vector<unsigned>> data;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Set bools to be emitted as their textual equivalents in English.
  std::cout.imbue(std::locale(std::cout.getloc(), [&]
  {
    std::boolalpha(std::cout);

    return new CodeEvalFacet();
  }()));

  for(SeatingPreferences preferences; inputStream >> preferences;)
  {
    std::cout << preferences.canAccommodate() << '\n';
  }
}
