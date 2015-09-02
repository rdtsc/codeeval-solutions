#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#ifndef __STDC_IEC_559__
#error
#endif

typedef struct
{
  unsigned id;
  double x, y;
} Pin;

static bool pinUnserialize(Pin* const pin, FILE* const inputStream)
{
  assert(pin && inputStream && !ferror(inputStream));

  #define ARGUMENT_PACK &pin->id, &pin->x, &pin->y

  if(fscanf(inputStream, "%u%*[^(](%lf, %lf)", ARGUMENT_PACK) != 3)
  {
    return false;
  }

  #undef ARGUMENT_PACK

  static const double radians = 0.01745329251994329576; // ~(Pi / 180)

  pin->x *= radians;
  pin->y *= radians;

  return true;
}

static double pinGetDistance(const Pin* const a, const Pin* const b)
{
  assert(a && b);

  if(a->id == b->id) return 0;

  static const double meanPlanetaryDiameter = 12742.0;

  // As per: https://en.wikipedia.org/wiki/Haversine_formula
  const double lhs = sin((b->x - a->x) / 2),
               rhs = sin((b->y - a->y) / 2);

  const double phi    = (lhs * lhs),
               lambda = (rhs * rhs);

  return meanPlanetaryDiameter * asin
  (
    sqrt(phi + cos(a->x) * cos(b->x) * lambda)
  );
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

  Pin pins[16] = {{0}};
  size_t pinCount = 0;

  // Fetch pins.
  while(pinUnserialize(&pins[pinCount], inputStream))
  {
    ++pinCount;

    assert(pinCount < (sizeof pins / sizeof *pins));
  }

  // Starting pin should have an ID of 1.
  assert((pinCount > 1) && (pins->id == 1));

  // VLAs yield a slightly better memory score on CodeEval than straight
  // [m,c]alloc. Shouldn't rely on this in C11 and onwards, though.
  double pinDistances[pinCount + 1][pinCount + 1];
  memset(pinDistances, 0, sizeof pinDistances);

  // Cache pin distances relative to each other.
  for(size_t y = 1; y <= pinCount; ++y)
  for(size_t x = 1; x <= pinCount; ++x)
  {
    pinDistances[y][x] = pinGetDistance(&pins[y - 1], &pins[x - 1]);
  }

  // Ditto regarding VLAs.
  unsigned pinIds[pinCount];
  memset(pinIds, 0, sizeof pinIds);

  // Compile the initial route.
  for(size_t i = 0; i < pinCount; ++i)
  {
    // The supplied pins appear to be already sorted and have adjacent IDs.
    assert((pinIds[i ? (i - 1) : 0] + 1) == pins[i].id);

    pinIds[i] = pins[i].id;
  }

  struct
  {
    double length;
    unsigned pins[sizeof pins / sizeof *pins];
  } shortestRoute = {.length = DBL_MAX};

  // Only have to deal with ~10 locations, so brute force should be okay.
  do
  {
    double routeLength = 0;

    // Current variant route length.
    for(size_t i = 0; i < (pinCount - 1); ++i)
    {
      routeLength += pinDistances[pinIds[i]][pinIds[i + 1]];

      if(routeLength > shortestRoute.length)
      {
        routeLength = DBL_MAX;
        break;
      }
    }

    if(routeLength < shortestRoute.length)
    {
      shortestRoute.length = routeLength;
      memcpy(shortestRoute.pins, pinIds, sizeof pinIds);
    }
  }
  while(nextPermutation(&pinIds[1], &pinIds[pinCount]));

  for(size_t i = 0; i < pinCount; ++i)
  {
    printf("%u\n", shortestRoute.pins[i]);
  }

  // The CRT takes care of cleanup.
}
