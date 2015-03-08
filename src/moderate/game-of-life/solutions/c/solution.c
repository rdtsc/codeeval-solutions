#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
  size_t width;
  size_t height;

  #define MAX_BOARD_ROWS 100

  char data[MAX_BOARD_ROWS][MAX_BOARD_ROWS + 1];
  char snapshot[MAX_BOARD_ROWS][MAX_BOARD_ROWS + 1];

  #undef MAX_BOARD_ROWS
} GameOfLife;

static unsigned golGetNeighborCount(const GameOfLife* const board,
                                    const size_t x, const size_t y)
{
  assert(board && board->width && board->height);
  assert(x < board->width && y < board->height);

  const size_t boardWidth  = board->width,
               boardHeight = board->height;

  #define IS_ALIVE(xOffset, yOffset)                             \
    (((!x && (xOffset) < 0) || (x + (xOffset) >= boardWidth)  || \
    (  !y && (yOffset) < 0) || (y + (yOffset) >= boardHeight)) ? \
    (0) : (board->data[y + (yOffset)][x + (xOffset)] == '*'))

  /*
    Possible neighbor position offsets.

    <A> -1 + + +
    <B>  0 + @ +
    <C>  1 + + +
          -1 0 1
  */

  return IS_ALIVE(-1, -1) + IS_ALIVE(0, -1) + IS_ALIVE(+1, -1) + // <A>
         IS_ALIVE(-1,  0) + (0000000000000) + IS_ALIVE(+1,  0) + // <B>
         IS_ALIVE(-1, +1) + IS_ALIVE(0, +1) + IS_ALIVE(+1, +1);  // <C>

  #undef IS_ALIVE
}

static void golTick(GameOfLife* const board)
{
  assert(board && board->width && board->height);

  for(size_t y = 0; y < board->height; ++y)
  for(size_t x = 0; x < board->width; ++x)
  {
    const unsigned neighbors = golGetNeighborCount(board, x, y);

    // Birth.
    if(neighbors == 3) board->snapshot[y][x] = '*';

    // Death
    else if(neighbors < 2 || neighbors > 3) board->snapshot[y][x] = '.';
  }

  memcpy(board->data, board->snapshot, sizeof board->snapshot);
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[4096] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  GameOfLife world = {0};

  for(char lineBuffer[128] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    memcpy(world.data[world.height++], lineBuffer, sizeof *world.data - 1);

    assert(world.height <= sizeof world.data / sizeof *world.data);
  }

  world.width = strlen(*world.data);

  assert(world.width);

  // Remove trailing newlines from the first slice and adjust the world's
  // slice size accordingly.
  for(char* c = *world.data + world.width - 1; *c && *c == '\n'; *c-- = '\0')
  {
    --world.width;

    assert(world.width);
  }

  // Normalize all remaining world slices.
  for(size_t i = 1; i < world.height; ++i)
  {
    world.data[i][world.width] = '\0';

    assert(strlen(world.data[i]) == world.width);
  }

  memcpy(world.snapshot, world.data, sizeof world.data);

  // Jump to the 10th generation.
  for(size_t i = 0; i < 10; ++i) golTick(&world);

  for(size_t i = 0; i < world.height; ++i) puts(world.data[i]);

  // The CRT takes care of cleanup.
}
