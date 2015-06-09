#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef enum {UNKNOWN, UP, DOWN, LEFT, RIGHT} Move;

typedef struct
{
  size_t width;
  unsigned cells[10][10];
} Board;

// Yields a small gain in performance over sscanf and friends in CE's
// environment due to naive parsing rules and lack of locale awareness
// on our part.
static inline unsigned extractUint(const char** string)
{
  assert(string && *string);

  unsigned result = 0;
  const char* c = *string;

  #define IS_PAYLOAD() (*c >= '0' && *c <= '9')

  // Eat anything that's not a digit.
  while(*c && !IS_PAYLOAD()) ++c;

  for(;*c; ++c)
  {
    if(!IS_PAYLOAD())
    {
      ++c;
      break;
    }

    else result = (result * 10) + (*c - '0');
  }

  #undef IS_PAYLOAD

  *string = c;

  return result;
}

// Ditto regarding performance.
static void boardUnserialize(const char* const state,
                             Move* const direction,
                             Board* const board)
{
  assert(state && *state && direction && board);

  const char* cursor = state;

  // Get direction.
  {
    while(cursor && *cursor == ' ') ++cursor;

    #define DETECT(Enumerator) \
      (*cursor == (*#Enumerator)) ? (Enumerator)

    *direction = DETECT(UP)    :
                 DETECT(DOWN)  :
                 DETECT(LEFT)  :
                 DETECT(RIGHT) : UNKNOWN;

    #undef DETECT

    assert(*direction != UNKNOWN);

    // Eat direction, including delimiter.
    if((cursor = strchr(cursor, ';'))) ++cursor;

    assert(cursor);
  }

  const size_t boardWidth = extractUint(&cursor);

  // As per the problem statement.
  assert(*cursor && boardWidth >= 4 && boardWidth <= 10);

  board->width = boardWidth;

  // Have to perform boardWidth^2 reads.
  const size_t opCount = (boardWidth * boardWidth);

  for(size_t i = 0, y = 0; (i < opCount) && *cursor; ++i)
  {
    const unsigned cell = extractUint(&cursor);

    // Values should be 2^n, as per the game's rules.
    assert(!(cell & (cell - 1)));

    const size_t x = (i % boardWidth);

    board->cells[y][x] = cell;

    if((x + 1) == boardWidth) ++y;
  }
}

static size_t boardReduceRow(unsigned* const row,
                             const size_t colCount,
                             Move direction)
{
  assert(row && colCount && direction != UNKNOWN);

  // "Rotate" the row 90 degrees CCW, if required.
  direction = (direction == UP)   ? (LEFT)  :
              (direction == DOWN) ? (RIGHT) :
              (direction);

  // Sanity check. Unrestricted VLAs can lead to UB if allocation fails.
  assert(colCount <= 10);

  // VLAs yield a slightly better memory score on CodeEval than straight
  // [m,c]alloc. Shouldn't rely on this in C11 and onwards, though.
  unsigned reducedRow[colCount];
  memset(reducedRow, 0, sizeof reducedRow);

  size_t reducedRowSize = 0;

  switch(direction)
  {
    case LEFT: for(size_t i = 0; i < colCount; ++i)
    {
      const size_t j = (i + 1);

      if((j < colCount) && (row[i] == row[j]))
      {
        reducedRow[reducedRowSize++] = (row[i++] * 2);
      }

      else reducedRow[reducedRowSize++] = row[i];
    }
    break;

    case RIGHT:
    {
      for(int i = (colCount - 1); i >= 0; --i)
      {
        int j = (i - 1);

        if((j >= 0) && (row[i] == row[j]))
        {
          reducedRow[reducedRowSize++] = (row[i--] * 2);
        }

        else reducedRow[reducedRowSize++] = row[i];
      }

      for(int i = 0, j = (reducedRowSize - 1); j > i; ++i, --j)
      {
        const unsigned placeholder = reducedRow[i];

        reducedRow[i] = reducedRow[j];
        reducedRow[j] = placeholder;
      }
    }
    break;

    default: assert(false);
  }

  memcpy(row, reducedRow, sizeof reducedRow);

  return reducedRowSize;
}

static void boardMoveVertically(Board* const board, const Move direction)
{
  assert(board && board->width && direction != UNKNOWN);

  for(size_t x = 0; x < board->width; ++x)
  {
    unsigned scratch[sizeof board->cells / sizeof *board->cells] = {0};
    size_t scratchSize = 0;

    for(size_t y = 0; y < board->width; ++y)
    {
      const unsigned cell = board->cells[y][x];

      if(cell) scratch[scratchSize++] = cell;
    }

    // Non-empty column.
    if(scratchSize)
    {
      scratchSize = boardReduceRow(scratch, scratchSize, direction);

      // Empty this column.
      for(size_t y = 0; y < board->width; ++y) board->cells[y][x] = 0;

      if(direction == UP) for(size_t i = 0, y = 0; i < scratchSize; ++i)
      {
        board->cells[y++][x] = scratch[i];
      }

      else for(size_t i = scratchSize, z = board->width; i-- && z--;)
      {
        board->cells[z][x] = scratch[i];
      }
    }
  }
}

static void boardMoveHorizontally(Board* const board, const Move direction)
{
  assert(board && board->width && direction != UNKNOWN);

  for(size_t y = 0; y < board->width; ++y)
  {
    unsigned scratch[sizeof board->cells / sizeof *board->cells] = {0};
    size_t scratchSize = 0;

    for(size_t x = 0; x < board->width; ++x) if(board->cells[y][x])
    {
      assert(scratchSize < (sizeof scratch / sizeof *scratch));

      scratch[scratchSize++] = board->cells[y][x];
    }

    // Non-empty row.
    if(scratchSize)
    {
      scratchSize = boardReduceRow(scratch, scratchSize, direction);

      // Empty this row.
      memset(board->cells[y], 0, board->width * sizeof(unsigned));

      if(direction == LEFT) for(size_t x = 0; x < scratchSize; ++x)
      {
        board->cells[y][x] = scratch[x];
      }

      else for(size_t x = scratchSize, z = board->width; x-- && z--;)
      {
        board->cells[y][z] = scratch[x];
      }
    }
  }
}

static void boardPerformMove(Board* const board, const Move direction)
{
  assert(board && board->width && direction != UNKNOWN);

  switch(direction)
  {
    case UP:   case DOWN:  boardMoveVertically(board, direction);   break;
    case LEFT: case RIGHT: boardMoveHorizontally(board, direction); break;

    default: assert(false);
  }
}

// A bit faster than printf in this use case.
static inline size_t appendTo(char* buffer, const unsigned n)
{
  assert(buffer);

  const char* const bufferBegin = buffer;

  #define THRESHOLD(Value) n < (1e##Value) ? (1e##Value) / 10

  const unsigned divisor = THRESHOLD(1) : THRESHOLD(2) :
                           THRESHOLD(3) : THRESHOLD(4) :
                           THRESHOLD(5) : 0;

  #undef THRESHOLD

  for(unsigned i = divisor; i; i /= 10)
  {
    *buffer++ = ('0' + (n / i) % 10);
  }

  return (buffer - bufferBegin);
}

static void boardDumpState(const Board* const board, FILE* const outputStream)
{
  assert(board && board->width && outputStream && !ferror(outputStream));

  static char outputBuffer[1024] = "";
  char* out = outputBuffer;

  for(size_t y = 0; y < board->width;)
  {
    out += appendTo(out, board->cells[y][0]);

    for(size_t x = 1; x < board->width; ++x)
    {
      *out++ = ' ';

      out += appendTo(out, board->cells[y][x]);
    }

    if(++y != board->width) *out++ = '|';
  }

  *out++ = '\n';
  *out++ = '\0';

  fputs(outputBuffer, outputStream);
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[8192] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    Move direction = UNKNOWN;
    Board board = {0};

    boardUnserialize(lineBuffer, &direction, &board);
    boardPerformMove(&board, direction);
    boardDumpState(&board, stdout);
  }

  // The CRT takes care of cleanup.
}
