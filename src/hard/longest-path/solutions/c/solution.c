#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

static unsigned getChainLengthAt(const char board[][6],
                                 const size_t boardWidth,
                                 const int x, const int y,
                                 uint_least32_t history)
{
  unsigned chainLength = 0;

  // Guard bounds.
  if(x < 0 || y < 0 || (size_t)x >= boardWidth || (size_t)y >= boardWidth)
  {
    return chainLength;
  }

  const size_t letterIndex = (board[y][x] - 'a');

  // We've already encountered this letter type before.
  if(history & (1 << letterIndex)) return chainLength;

  // The current letter type should not be used again.
  history |= (1 << letterIndex);

  #define TRY_NEXT(xOffset, yOffset) do {                              \
    const int newX = x + (xOffset), newY = y + (yOffset);              \
    const unsigned newLength =                                         \
      (getChainLengthAt(board, boardWidth, newX, newY, history) + 1);  \
    chainLength = (newLength > chainLength ? newLength : chainLength); \
  } while(false)

  // Proceed to horizontal and vertical neighbors.
  TRY_NEXT(+1, 0); TRY_NEXT(0, +1);
  TRY_NEXT(-1, 0); TRY_NEXT(0, -1);

  #undef TRY_NEXT

  return chainLength;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[128] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[64] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    size_t lineLength = 0;

    // Combining strlen and content assertion.
    for(const char* c = lineBuffer; *c && *c != '\n'; ++c, ++lineLength)
    {
      assert(islower(*c));
    }

    const size_t boardWidth = sqrt(lineLength);

    // As per the problem statement.
    assert(boardWidth >= 2 && boardWidth <= 6);

    char board[6][6] = {""};

    // Reconstruct the board.
    {
      const char* cursor = lineBuffer;

      for(size_t y = 0; y < boardWidth; ++y)
      for(size_t x = 0; x < boardWidth; ++x)
      {
        board[y][x] = *cursor++;
      }
    }

    unsigned result = 0;

    for(size_t y = 0; y < boardWidth; ++y)
    for(size_t x = 0; x < boardWidth; ++x)
    {
      const unsigned history = 0;

      const unsigned chainLength =
        getChainLengthAt((const char(*)[])board, boardWidth, x, y, history);

      if(chainLength > result) result = chainLength;
    }

    printf("%u\n", result);
  }

  // The CRT takes care of cleanup.
}
