#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

static inline bool hasUniqueDigits(unsigned n)
{
  unsigned short history = 0;

  for(;n; n /= 10)
  {
    const unsigned char digit = (n % 10);

    #define BIT_SET_AT(Position) (history  & (1 << (Position)))
    #define SET_BIT_AT(Position) (history |= (1 << (Position)))

    if(BIT_SET_AT(digit)) return false;

    SET_BIT_AT(digit);

    #undef SET_BIT_AT
    #undef BIT_SET_AT
  }

  return true;
}

static bool isMagicNumber(const unsigned n)
{
  assert(n >= 1 && n <= 1e4);

  if(!hasUniqueDigits(n)) return false;

  // Guaranteed that N is at most 5 digits wide by the problem statement.
  unsigned char digits[5] = {0};

  #define THRESHOLD(Value) n < (1e##Value) ? (Value)

  // Should be a bit faster than a call to log10.
  const size_t digitCount = THRESHOLD(1) : THRESHOLD(2) :
                            THRESHOLD(3) : THRESHOLD(4) :
                            THRESHOLD(5) : 0;

  assert(digitCount && digitCount <= (sizeof digits / sizeof *digits));

  #undef THRESHOLD

  // Stuff N's digits into the `digits` array.
  {
    unsigned m = n;

    for(size_t i = digitCount; i--; m /= 10) digits[i] = (m % 10);
  }

  unsigned char uniquenessMask = 1;

  for(size_t i = *digits % digitCount; i;)
  {
    #define BIT_SET_AT(Position) (uniquenessMask  & (1 << (Position)))
    #define SET_BIT_AT(Position) (uniquenessMask |= (1 << (Position)))

    // We've been at this index before, therefore not a "magic" number.
    if(BIT_SET_AT(i)) return false;

    // First visit.
    SET_BIT_AT(i);
    i = (digits[i] + i) % digitCount;

    #undef SET_BIT_AT
    #undef BIT_SET_AT
  }

  // All digits should have been visited exactly once for this to be
  // considered a "magic" number.
  return (uniquenessMask == (1 << digitCount) - 1);
}

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

  *buffer++ = ' ';

  return (buffer - bufferBegin);
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[2048] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  struct
  {
    unsigned lower, upper;
  } ranges[32]   = {{0}},
    globalBounds = {.lower = UINT_MAX};

  size_t rangeCount = 0;

  // Fetch all the range pairs and get the lowest and highest values.
  for(;fscanf(inputStream, "%u %u", &ranges[rangeCount].lower,
                                    &ranges[rangeCount].upper) == 2;
                                    ++rangeCount)
  {
    assert(rangeCount < sizeof ranges / sizeof *ranges);
    assert(ranges[rangeCount].lower <= ranges[rangeCount].upper);
    assert(ranges[rangeCount].lower >= 1);

    #define MIN(x, y) ((x) < (y) ? (x) : (y))
    #define MAX(x, y) ((x) > (y) ? (x) : (y))

    globalBounds.lower = MIN(globalBounds.lower, ranges[rangeCount].lower);
    globalBounds.upper = MAX(globalBounds.upper, ranges[rangeCount].upper);

    #undef MIN
    #undef MAX
  }

  assert(globalBounds.upper && (globalBounds.upper >= globalBounds.lower));

  // There are only 89 "magic" numbers <= 1e4.
  char magicNumbers[512] = " ";

  size_t magicIndexA[10001] = {0},
         magicIndexB[10001] = {0};

  // Pre-compute and pre-format the result set for increased performance.
  {
    char* out = magicNumbers;
    size_t breakpoint = 0;

    for(size_t i = globalBounds.lower; i <= globalBounds.upper; ++i)
    {
      if(isMagicNumber(i))
      {
        magicIndexA[i] = (out - magicNumbers);

        out += appendTo(out, i);

        // Spaces act as snip guidelines.
        magicIndexB[i] = breakpoint = (out - magicNumbers) - 1;
      }

      else magicIndexA[i] = magicIndexB[i] = breakpoint;
    }
  }

  // Input range playback.
  for(size_t i = 0; i < rangeCount; ++i)
  {
    const unsigned lowerBound = ranges[i].lower,
                   upperBound = ranges[i].upper;

    const size_t begin = magicIndexA[lowerBound],
                 end   = magicIndexB[upperBound];

    // Temporarily snip the end of the sequence.
    magicNumbers[end] = '\0';

    const char* result = magicNumbers + begin;

    const bool isRangeEmpty = ((lowerBound != upperBound) && (begin == end)) ||
                              !*result;

    puts(isRangeEmpty ? "-1" : (*result == ' ' ? ++result : result));

    // Glue the sequence back together.
    magicNumbers[end] = ' ';
  }

  // The CRT takes care of cleanup.
}
