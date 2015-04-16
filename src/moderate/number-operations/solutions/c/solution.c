#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static inline int strictWeakOrdering(const void* const lhs,
                                     const void* const rhs)
{
  assert(lhs && rhs);

  const unsigned x = *(unsigned*)lhs,
                 y = *(unsigned*)rhs;

  return (x < y) ? (-1) : (x > y);
}

static inline bool nextPermutation(unsigned* const begin, unsigned* const end)
{
  assert(begin && end && end > begin);

  const size_t rangeSize = end - begin - 1;
  size_t i = rangeSize, j = i;

  // Find the largest index I such that array[I] < array[I + 1]...
  while(i && begin[i - 1] >= begin[i]) --i;

  // ...if no such index exists, the permutation is the last permutation.
  if(!i) return false;

  // Find the largest index J greater than I such that array[I] < array[J].
  for(--i; begin[j] <= begin[i]; --j);

  #define SWAP(lhs, rhs) do { \
    const unsigned oldLhs = (lhs); (lhs) = (rhs); (rhs) = oldLhs; \
  } while(false)

  // Swap the value of array[I] with [J].
  SWAP(begin[i], begin[j]);

  // Reverse the sequence from array[I + 1] up to the last element, inclusive.
  for(++i, j = rangeSize; j > i; ++i, --j) SWAP(begin[i], begin[j]);

  #undef SWAP

  return true;
}

static bool isValidHand(const int targetCardSum,
                        const unsigned* const hand,
                        const size_t cardCount)
{
  assert(hand && cardCount);

  enum Operators {ADD, SUB, MUL};
  unsigned operators[4] = {ADD};

  const size_t operatorCount = sizeof operators / sizeof *operators;

  assert(operatorCount + 1 == cardCount);

  // Attempt to find a solution within the 3^4 possible operator arrangements.
  for(size_t i = 0; i < 81; ++i)
  {
    // Compute the sum of our hand based on the currently selected operators.
    {
      int handValue = *hand;

      for(size_t i = 0; i < operatorCount; ++i) switch(operators[i])
      {
        case ADD: handValue += hand[i + 1]; break;
        case SUB: handValue -= hand[i + 1]; break;
        case MUL: handValue *= hand[i + 1]; break;
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

  static char stdoutBuffer[256] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[32] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    unsigned cards[5] = {0};
    size_t cardCount = 0;

    // Yields a small gain in performance over sscanf and friends in CE's
    // environment due to naive parsing rules and lack of locale awareness on
    // our part.
    {
      unsigned card = 0;

      for(const char* c = lineBuffer;; ++c)
      {
        if(*c >= '0' && *c <= '9') card = (card * 10) + (*c - '0');

        else if(*c == ' ' || *c == '\n' || !*c)
        {
          assert(cardCount < sizeof cards / sizeof *cards);

          cards[cardCount++] = card;

          card = 0;

          if(*c == '\n' || !*c) break;
        }
      }

      assert(cardCount == 5);
    }

    qsort(cards, cardCount, sizeof *cards, strictWeakOrdering);

    bool foundSolution = false;

    do
    {
      foundSolution = isValidHand(42, cards, cardCount);
    }
    while(!foundSolution && nextPermutation(cards, cards + cardCount));

    puts(foundSolution ? "YES": "NO");
  }

  // The CRT takes care of cleanup.
}
