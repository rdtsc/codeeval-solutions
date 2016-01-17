#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <unordered_map>

class Hand
{
  public: Hand() : value()
  {
    assert(this->cards.size() == 5);
  }

  public: bool operator<(const Hand& other) const
  {
    assert(this->value && other.value);

    // The hand with the highest-valued card wins.
    if(this->value == other.value)
    {
      for(std::size_t i = 5; i--;)
      {
        const auto a = this->cards[i].value,
                   b = other.cards[i].value;

        if(a != b) return (a < b);
      }

      return false;
    }

    return (this->value < other.value);
  }

  public: bool operator>(const Hand& other) const
  {
    return (other < *this);
  }

  public: bool operator==(const Hand& other) const
  {
    // TODO: Optimize.
    return !((*this < other) || (other < *this));
  }

  private: unsigned getValue() const
  {
    assert(std::is_sorted(this->cards.cbegin(), this->cards.cend()));

    const auto allPairsAre = [&](bool(* const condition)(const Card&,
                                                         const Card&))
    {
      assert(condition);

      const auto cardsBegin = this->cards.cbegin(),
                 cardsEnd   = this->cards.cend();

      const auto tail = std::prev(cardsEnd);

      for(auto i = cardsBegin; i != tail; ++i)
      {
        if(!condition(*i, *std::next(i))) return false;
      }

      return true;
    };

    const auto histogram = [&]
    {
      std::unordered_map<unsigned char, unsigned> result;

      for(const auto& card : this->cards) ++result[card.value];

      return result;
    }();

    const auto getValueDupeCount = [&](const unsigned count)
    {
      const auto begin = histogram.cbegin(),
                 end   = histogram.cend();

      using value_type = decltype(histogram)::value_type;

      return std::count_if(begin, end, [&](const value_type& hit)
      {
        return (hit.second == count);
      });
    };

    const auto pairCount = getValueDupeCount(2);

    // Two cards of the same value.
    const auto isOnePair = (pairCount == 1);

    // Two different pairs.
    const auto isTwoPairs = (pairCount == 2);

    // Three cards of the same value.
    const auto isThreeOfAKind = getValueDupeCount(3);

    // All cards have consecutive values.
    const auto isStraight = allPairsAre([](const Card& lhs, const Card& rhs)
    {
      return ((rhs.value - lhs.value) == 1);
    });

    // All cards belong to the same suit.
    const auto isFlush = allPairsAre([](const Card& lhs, const Card& rhs)
    {
      return (lhs.suit == rhs.suit);
    });

    // Three of a kind and a pair.
    const auto isFullHouse = (isThreeOfAKind && isOnePair);

    // Four cards of the same value.
    const auto isFourOfAKind = getValueDupeCount(4);

    // All cards are consecutive values of the same suit.
    const auto isStraightFlush = (isStraight && isFlush);

    // Ten, Jack, Queen, King, and an Ace in same suit.
    const auto isRoyalFlush =
      (isStraightFlush && (this->cards.front().value == 10));

    #define SIFT(Condition, Value) \
      if((is##Condition)) return (Value)

    SIFT(RoyalFlush,    23);
    SIFT(StraightFlush, 22);
    SIFT(FourOfAKind,   21);
    SIFT(FullHouse,     20);
    SIFT(Flush,         19);
    SIFT(Straight,      18);
    SIFT(ThreeOfAKind,  17);
    SIFT(TwoPairs,      16);
    SIFT(OnePair,       15);

    #undef SIFT

    // Highest value card.
    return this->cards.back().value;
  }

  private: friend std::istream& operator>>(std::istream& inputStream,
                                           Hand& self)
  {
    self.value = 0;

    auto& cards = self.cards;
    std::size_t readCount = 0;

    for(auto& card : cards)
    {
      if(!(inputStream >> card)) return inputStream;

      ++readCount;
    }

    assert(readCount == cards.size());

    std::sort(cards.begin(), cards.end());

    self.value = self.getValue();

    return inputStream;
  }

  private: struct Card
  {
    Card() : suit(), value() {}

    bool operator<(const Card& other) const
    {
      return (this->value < other.value);
    }

    friend std::istream& operator>>(std::istream& inputStream, Card& self)
    {
      char value = '\0', suit = '\0';

      if(!(inputStream >> value >> suit)) return inputStream;

      assert(std::string("123456789TJQKA").find(value) != std::string::npos);
      assert(std::string("CDHS").find(suit) != std::string::npos);

      static const auto values = []
      {
        // TODO: Reduce size by offsetting against zero.
        std::array<unsigned char, 'Z'> result = {0};

        std::iota(&result['2'], &result[':'], 1);

        #define MAP(Symbol, Value) result[*(#Symbol)] = (Value)

        MAP(Clubs,    1);
        MAP(Diamonds, 2);
        MAP(Hearts,   3);
        MAP(Spades,   4);
        MAP(Ten,     10);
        MAP(Jack,    11);
        MAP(Queen,   12);
        MAP(King,    13);
        MAP(Ace,     14);

        #undef MAP

        return result;
      }();

      self.suit  = values[suit];
      self.value = values[value];

      return inputStream;
    }

    unsigned char suit;
    unsigned char value;
  };

  private: unsigned value;
  private: std::array<Card, 5> cards;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  const auto getWinner = [](const Hand& lhs, const Hand& rhs)
  {
    return (lhs == rhs ? "none" : (lhs > rhs ? "left" : "right"));
  };

  for(Hand lhs, rhs; inputStream >> lhs >> rhs;)
  {
    std::cout << getWinner(lhs, rhs) << '\n';
  }
}
