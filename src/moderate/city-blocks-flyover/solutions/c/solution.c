#include <assert.h>
#include <stddef.h>
#include <stdio.h>

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

static unsigned getFlightPathIntersections(const size_t streetCount,
                                           const unsigned streets[streetCount],
                                           const size_t avenueCount,
                                           const unsigned avenues[avenueCount])
{
  assert(streetCount && streets && avenueCount && avenues);

  unsigned result = 0;

  // Values appear to be pre-sorted in the input.
  const float slope = (float)avenues[avenueCount - 1] /
                             streets[streetCount - 1];

  for(size_t i = 0; i < (streetCount - 1); ++i)
  {
    const float thisStreet = (streets[i + 0] * slope),
                nextStreet = (streets[i + 1] * slope);

    for(size_t j = 0; j < (avenueCount - 1); ++j)
    {
      const unsigned thisAvenue = avenues[j + 0],
                     nextAvenue = avenues[j + 1];

      if((thisStreet < nextAvenue) && (nextStreet > thisAvenue)) ++result;
    }
  }

  return result;
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

  for(char lineBuffer[1024] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    unsigned streets[128] = {0},
             avenues[128] = {0};

    size_t streetCount = 0,
           avenueCount = 0;

    // Unserialize axis markers.
    {
      const char* cursor = lineBuffer;

      // Fetch streets.
      while(*cursor != ' ')
      {
        assert(streetCount < (sizeof streets / sizeof *streets));

        streets[streetCount++] = extractUint(&cursor);
      }

      // Fetch avenues.
      while(*cursor && *cursor != '\n')
      {
        assert(avenueCount < (sizeof avenues / sizeof *avenues));

        avenues[avenueCount++] = extractUint(&cursor);
      }
    }

    printf("%u\n", getFlightPathIntersections(streetCount, streets,
                                              avenueCount, avenues));
  }

  // The CRT takes care of cleanup.
}
