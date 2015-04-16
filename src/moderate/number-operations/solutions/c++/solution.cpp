#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

template<typename T, typename U>
static bool isValidHand(const T targetCardSum, const std::vector<U>& hand)
{
  enum Operators {ADD, SUB, MUL};
  unsigned operators[4] = {};

  static constexpr auto operatorCount = sizeof operators / sizeof *operators;

  assert(operatorCount + 1 == hand.size());

  // Attempt to find a solution within the 3^4 possible operator arrangements.
  for(std::size_t i = 0; i < 81; ++i)
  {
    // Compute the sum of our hand based on the currently selected operators.
    {
      T handValue = static_cast<T>(hand[0]);

      for(std::size_t i = 0; i < operatorCount; ++i) switch(operators[i])
      {
        #define NEIGHBOR() static_cast<T>(hand[i + 1])

        case ADD: handValue += NEIGHBOR(); break;
        case SUB: handValue -= NEIGHBOR(); break;
        case MUL: handValue *= NEIGHBOR(); break;

        #undef NEIGHBOR
      }

      if(handValue == targetCardSum) return true;
    }

    // Select the next combination of operators by counting in base 3.
    for(int j = 3; j >= 0; --j)
    {
      ++operators[j];

      if(operators[j] != 3) break;

      operators[j] = 0;
    }
  }

  return false;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::vector<unsigned> cards;
  std::istringstream tokenizer;

  for(std::string line; std::getline(inputStream, line);)
  {
    cards.clear();
    tokenizer.clear();
    tokenizer.str(line);

    for(decltype(cards)::value_type card; tokenizer >> card;)
    {
      cards.push_back(card);
    }

    assert(cards.size() == 5);

    const auto cardsBegin = cards.begin(),
               cardsEnd   = cards.end();

    std::sort(cardsBegin, cardsEnd);

    bool foundSolution = false;

    do
    {
      foundSolution = ::isValidHand(42, cards);
    }
    while(!foundSolution && std::next_permutation(cardsBegin, cardsEnd));

    std::cout << (foundSolution ? "YES" : "NO") << '\n';
  }
}
