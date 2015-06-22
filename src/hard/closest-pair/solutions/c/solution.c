#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

// Non-STDC.
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

typedef struct Point2d {unsigned x, y;} Point2d;

typedef struct
{
  int handle;
  size_t length;
  const char* data;
} FileMapping;

// Uses non-standard APIs, but has good performance characteristics given the
// size of the input files for this problem. When submitting to CodeEval,
// _GNU_SOURCE should be defined.
static FileMapping getFileMapping(const char* const target)
{
  assert(target && *target);

  const int fileHandle = open(target, O_RDONLY);

  assert(fileHandle != -1 && "Failed to acquire file descriptor.");

  struct stat stats = {0};

  if(fstat(fileHandle, &stats) < 0) assert(false && "Failed to get file size.");

  const int mappingAttributes = (MAP_PRIVATE | MAP_POPULATE);

  const char* const data =
    mmap(NULL, stats.st_size, PROT_READ, mappingAttributes, fileHandle, 0);

  assert(data != MAP_FAILED && "Failed to map file into memory.");

  const FileMapping result =
  {
    .handle = fileHandle,
    .length = stats.st_size,
    .data   = data
  };

  return result;
}

// Yields a small gain in performance over sscanf and friends in CE's
// environment due to naive parsing rules and lack of locale awareness
// on our part.
static unsigned extractUint(const char** string)
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

static unsigned getClosestPairSquaredDistance(const Point2d* const points,
                                              const size_t pointCount)
{
  assert(points && pointCount);

  unsigned result = UINT_MAX;

  const Point2d* const pointsEnd = (points + pointCount);

  // HACK: It appears that CodeEval only supplies ~500 points (at most) per
  // data set. Consequently, a naive brute-force search is fast enough to get
  // a high point score. If performance ever becomes an issue, this standard
  // proximity problem can be solved via a recursive divide & conquer search
  // in O(n log n) time.
  for(const Point2d* i = points;  i < pointsEnd; ++i)
  for(const Point2d* j = (i + 1); j < pointsEnd; ++j)
  {
    int deltaX = ((int)i->x - (int)j->x),
        deltaY = ((int)i->y - (int)j->y);

    deltaX *= deltaX;
    deltaY *= deltaY;

    const unsigned currentDistance = (deltaX + deltaY);

    if(result > currentDistance) result = currentDistance;
  }

  return result;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[512] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FileMapping inputStream = getFileMapping(argv[1]);

  assert(inputStream.data != MAP_FAILED && "Failed to open input stream.");

  Point2d points[768] = {{0}};

  for(const char* cursor = inputStream.data; *cursor;)
  {
    const size_t expectedPointCount = extractUint(&cursor);

    // Dataset terminator is always zero.
    if(!expectedPointCount) break;

    // As per the problem statement.
    assert(expectedPointCount <= 10000);
    assert(expectedPointCount < (sizeof points / sizeof *points));

    for(size_t i = 0; i < expectedPointCount; ++i)
    {
      points[i].x = extractUint(&cursor);
      points[i].y = extractUint(&cursor);
    }

    const unsigned minDistance =
      getClosestPairSquaredDistance(points, expectedPointCount);

    // As per the problem statement.
    static const size_t threshold = (10000 * 10000);

    if(minDistance >= threshold) puts("INFINITY");

    else printf("%.4lf\n", sqrt((double)minDistance));
  }

  // The CRT takes care of cleanup.
}
