#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {int width, height;} Hole;
typedef struct {unsigned id; int width, height, depth;} Brick;

// Yields a small gain in performance over sscanf and friends in CE's
// environment due to naive parsing rules and lack of locale awareness
// on our part.
static inline int stoi(char** string)
{
  assert(string && *string);

  int result = 0;
  bool isNegative = false;

  char* c = *string;

  #define IS_PAYLOAD() ((*c >= '0' && *c <= '9') || (*c == '-'))

  // Eat anything that's not a digit or hyphen.
  while(*c && !IS_PAYLOAD()) ++c;

  for(;*c; ++c)
  {
    if(!IS_PAYLOAD())
    {
      ++c;
      break;
    }

    else if(*c == '-') isNegative = true;

    else result = (result * 10) + (*c - '0');
  }

  #undef IS_PAYLOAD

  *string = c;

  return ((isNegative) ? (-result) : (result));
}

static void getHoleMetrics(char** string, Hole* const hole)
{
  assert(string && *string && hole);

  int x[2] = {0}, y[2] = {0};

  #define GET_POINT(Index) do {                           \
    assert((Index) < (sizeof x / sizeof *x));             \
    x[(Index)] = stoi(string); y[(Index)] = stoi(string); \
  } while(false)

  GET_POINT(0);
  GET_POINT(1);

  #undef GET_POINT

  hole->width  = abs(x[0] - x[1]);
  hole->height = abs(y[0] - y[1]);
}

static void getBrickMetrics(char** string, Brick* const brick)
{
  assert(string && *string && brick);

  int x[2] = {0}, y[2] = {0}, z[2] = {0};

  #define GET_POINT(Index) do {               \
    assert((Index) < (sizeof x / sizeof *x)); \
    x[(Index)] = stoi(string);                \
    y[(Index)] = stoi(string);                \
    z[(Index)] = stoi(string);                \
  } while(false)

  brick->id = stoi(string);

  GET_POINT(0);
  GET_POINT(1);

  #undef GET_POINT

  brick->width  = abs(x[0] - x[1]);
  brick->height = abs(y[0] - y[1]);
  brick->depth  = abs(z[0] - z[1]);
}

static inline bool canFit(const Brick* const brick, const Hole* const hole)
{
  assert(brick && hole);

  const int w = brick->width,
            h = brick->height,
            d = brick->depth;

  const int holeWidth  = hole->width,
            holeHeight = hole->height;

  // Basic rotations of the brick.
  const Hole variant[] =
  {
    {w, h}, {h, w},
    {w, d}, {d, w},
    {h, d}, {d, h}
  };

  for(size_t i = 0; i < (sizeof variant / sizeof *variant); ++i)
  {
    if(holeWidth >= variant[i].width && holeHeight >= variant[i].height)
    {
      return true;
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

  for(char lineBuffer[512] = "", * cursor = lineBuffer;
      fgets(lineBuffer, sizeof lineBuffer, inputStream);
      cursor = lineBuffer)
  {
    Hole hole = {0};
    getHoleMetrics(&cursor, &hole);

    bool foundBricks = false;

    // Looks like the brick IDs always start with 1, but are not guaranteed to
    // appear in ascending order afterwards.
    unsigned brickId = 1;

    for(char* chunk = strtok(cursor, ";"); chunk; chunk = strtok(NULL, ";"))
    {
      Brick brick = {0};
      getBrickMetrics(&chunk, &brick);

      if(canFit(&brick, &hole))
      {
        if(foundBricks) putchar(',');

        printf("%d", brickId);

        foundBricks = true;
      }

      ++brickId;
    }

    if(!foundBricks) putchar('-');

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
