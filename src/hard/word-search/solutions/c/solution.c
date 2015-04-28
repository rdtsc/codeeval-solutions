#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct
{
  char data[3][4];
} Board;

static bool boardContainsWord(Board* const board,
                              const char* word,
                              const int x,
                              const int y)
{
  static const size_t boardWidth  = (sizeof *board->data),
                      boardHeight = (sizeof board->data / sizeof *board->data);

  // Guard bounds.
  if(x < 0 || y < 0 || (size_t)x >= boardWidth || (size_t)y >= boardHeight)
  {
    return false;
  }

  const char letter = board->data[y][x];

  // We've either been here before or there's a mismatch.
  if(letter == ' ' || letter != *word) return false;

  // Remove the match.
  board->data[y][x] = ' ';

  #define TRY_NEXT(xOffset, yOffset) \
    boardContainsWord(board, word, x + (xOffset), y + (yOffset))

  if(!*++word) return true;

  // Proceed to horizontal and vertical neighbors.
  return TRY_NEXT(-1, 0) || TRY_NEXT(+1, 0) ||
         TRY_NEXT(0, -1) || TRY_NEXT(0, +1);

  #undef TRY_NEXT
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

  typedef struct
  {
    size_t count;
    size_t values[3];
  } Position;

  static const Position positions[] =
  {
    /* [0] 1:A */ {.count = 2, {0,  8,  0}},
    /* [1] 2:B */ {.count = 1, {1,  0,  0}},
    /* [2] 3:C */ {.count = 2, {2,  6,  0}},
    /* [3] 4:D */ {.count = 1, {9,  0,  0}},
    /* [4] 5:E */ {.count = 3, {3, 10, 11}},
    /* [5] 6:F */ {.count = 1, {5,  0,  0}},
    /* [6] 7:S */ {.count = 2, {4,  7,  0}},
  };

  static const size_t letterIndex['Z' - 'A'] =
  {
    ['A' - 'A'] = 1, 2, 3, 4, 5, 6,
    ['S' - 'A'] = 7
  };

  for(char lineBuffer[16] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    // Bail early if the input doesn't make sense.
    {
      size_t wordLength = 0;
      bool isPlausible = true;

      // Same values as in the master position table.
      size_t frequencies[] = {2, 1, 2, 1, 3, 1, 2};

      for(const char* c = lineBuffer; *c && *c != '\n'; ++c, ++wordLength)
      {
        assert(*c >= 'A' && *c <= 'Z');

        const size_t key = (*c - 'A');

        // No such letter in the board or too many occurrences of
        // said letter in the input string.
        if(!letterIndex[key] || !frequencies[letterIndex[key] - 1])
        {
          isPlausible = false;
          break;
        }

        --frequencies[letterIndex[key] - 1];
      }

      if(!isPlausible)
      {
        puts("False");
        continue;
      }

      lineBuffer[wordLength] = '\0';
    }

    bool foundWord = false;

    {
      const Position* const root =
        &positions[letterIndex[*lineBuffer - 'A'] - 1];

      for(size_t i = 0; i < root->count; ++i)
      {
        // The search process is destructive.
        Board board =
        {{
          "ABCE",
          "SFCS",
          "ADEE"
        }};

        static const size_t width = (sizeof *board.data);

        if(boardContainsWord(&board, lineBuffer, root->values[i] % width,
                                                 root->values[i] / width))
        {
          foundWord = true;
          break;
        }
      }
    }

    puts(foundWord ? "True" : "False");
  }

  // The CRT takes care of cleanup.
}
