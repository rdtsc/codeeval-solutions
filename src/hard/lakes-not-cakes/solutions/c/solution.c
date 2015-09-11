#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
  size_t width;
  size_t height;

  #define MAX_WORLD_WIDTH  30
  #define MAX_WORLD_HEIGHT 30

  char data[MAX_WORLD_HEIGHT][MAX_WORLD_WIDTH + 1];
  bool breadcrumbs[MAX_WORLD_HEIGHT][MAX_WORLD_WIDTH];

  #undef MAX_WORD_HEIGHT
  #undef MAX_WORLD_WIDTH
} World;

static void worldUnserialize(const char* const data, World* const world)
{
  assert(data && *data && world);

  memset(world, 0, sizeof(World));

  const char* cursor = data;

  // Fetch first row to determine world width.
  for(;*cursor != '|'; ++cursor) switch(*cursor)
  {
    case 'o': case '#': world->data[0][world->width++] = *cursor;
  }

  // Fetch subsequent rows.
  for(size_t column = 0;; ++cursor)
  {
    if(*cursor == 'o' || *cursor == '#')
    {
      world->data[world->height][column++] = *cursor;
    }

    else if(*cursor == '|' || *cursor == '\n' || *cursor == EOF)
    {
      column = 0;
      ++world->height;

      if(*cursor == '\n' || *cursor == EOF) break;
    }
  }

  // As per the problem statement.
  assert((world->width  >= 03) && (world->height >= 03));
  assert((world->height <= 30) && (world->height <= 30));
}

static bool worldIsWaterCell(const World* const world, const int x, const int y)
{
  assert(world);

  // Guard world bounds.
  if((x < 0) || ((size_t)x >= world->width) ||
     (y < 0) || ((size_t)y >= world->height))
  {
    return false;
  }

  // Lake fragments are denoted by lowercase o's.
  return (!world->breadcrumbs[y][x] && (world->data[y][x] == 'o'));
}

static void worldVisitCellGroup(World* const world, const int x, const int y)
{
  assert(world && (x >= 0) && (y >= 0));

  world->breadcrumbs[y][x] = true;

  static struct {int x, y;} const offsets[] =
  {
    /*
      Possible neighbor position offsets.

      <A> -1 + + +
      <B>  0 + @ +
      <C>  1 + + +
            -1 0 1
    */

    {-1, -1}, {0, -1}, {1, -1}, // <A>
    {-1,  0}, /* @ */  {1,  0}, // <B>
    {-1,  1}, {0,  1}, {1,  1}  // <C>
  };

  static const size_t offsetCount = (sizeof offsets / sizeof *offsets);

  for(size_t i = 0; i < offsetCount; ++i)
  {
    const int newX = (x + offsets[i].x),
              newY = (y + offsets[i].y);

    if(worldIsWaterCell(world, newX, newY))
    {
      worldVisitCellGroup(world, newX, newY);
    }
  }
}

static unsigned worldGetLakeCount(World* const world)
{
  assert(world && world->width && world->height);

  // Clear history.
  memset(world->breadcrumbs, false, sizeof(world->breadcrumbs));

  unsigned lakeCount = 0;

  for(size_t y = 0; y < world->height; ++y)
  for(size_t x = 0; x < world->width;  ++x)
  {
    if(worldIsWaterCell(world, x, y))
    {
      ++lakeCount;
      worldVisitCellGroup(world, x, y);
    }
  }

  return lakeCount;
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

  World world = {0};

  for(char lineBuffer[2048] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    worldUnserialize(lineBuffer, &world);

    printf("%u\n", worldGetLakeCount(&world));
  }

  // The CRT takes care of cleanup.
}
