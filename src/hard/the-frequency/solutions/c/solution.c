#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {double x, y;} Point2d;

typedef struct {Point2d first, second;} Segment2d;

typedef const Point2d* const Point2dArg;

static inline bool isCounterclockwise(Point2dArg a, Point2dArg b, Point2dArg c)
{
  assert(a && b && c);

  return ((c->y - a->y) * (b->x - a->x)) >
         ((b->y - a->y) * (c->x - a->x));
}

static inline bool isIntersection(Point2dArg a, Point2dArg b, Point2dArg c,
                                  Point2dArg d, Point2dArg e, Point2dArg f)
{
  assert(a && b && c && d && e && f);

  return (isCounterclockwise(a, b, c) !=
          isCounterclockwise(d, e, f));
}

static inline bool segmentsIntersect(const Segment2d* const lhs,
                                     const Segment2d* const rhs)
{
  assert(lhs && rhs);

  Point2dArg a = &lhs->first, b = &lhs->second,
             c = &rhs->first, d = &rhs->second;

  return isIntersection(a, c, d, b, c, d) &&
         isIntersection(a, b, c, a, b, d);
}

static unsigned getFrequency(const int* const samples,
                             const size_t sampleCount,
                             const unsigned sampleRate)
{
  typedef int_fast64_t value_type;

  /*
    The sample's linear trend line has the form of: y = slope * x + intercept;

    Where:
    ------

    X = time slices;
    Y = amplitude samples;
    Z = sample count;

            Z * sum(X * Y) - sum(X) * sum(Y)
    slope = --------------------------------;
                Z * sum(X^2) - sum(X)^2


                sum(Y) - slope * sum(X)
    intercept = -----------------------;
                           Z
  */

  const size_t size = sampleCount;

  value_type amplitudeSum = 0;

  for(size_t i = 0; i < size; ++i) amplitudeSum += samples[i];

  // Arithmetic sum.
  const value_type timeSum = (size / 2) * (size + 1);

  double trendSlope = 0;

  // Compute waveform trend line slope.
  {
    value_type a = 0;

    // Compute A.
    {
      for(size_t i = 0, x = 0; i < size; ++i, ++x) a += (x * samples[i]);

      a *= size;
    }

    const value_type b = (amplitudeSum * timeSum);

    // Sum of squares.
    const value_type c = ((size * size) * (size + 1) * (2 * size + 1) / 6);

    const value_type d = (timeSum * timeSum);

    trendSlope = (double)(a - b) / (c - d);
  }

  const double trendIntercept =
    ((amplitudeSum - (trendSlope * timeSum)) / size);

  // Waveform trend line segment.
  const Segment2d trend =
  {
    {
      .x = 0,
      .y = trendIntercept
    },
    {
      .x = size,
      .y = trendSlope * size + trendIntercept
    }
  };

  unsigned intersections = 0;

  // Walk through the waveform's immediate segment neighbor pairs and check
  // whether their line segment intersects the trend line. Unoptimized.
  for(size_t i = 0; i < (size - 1); ++i)
  {
    const Segment2d waveformSegment =
    {
      {.x = i + 0, .y = samples[i + 0]},
      {.x = i + 1, .y = samples[i + 1]}
    };

    if(segmentsIntersect(&trend, &waveformSegment)) ++intersections;
  }

  const unsigned fHz = (intersections * (sampleRate / size) / 2);

  // As per the problem statement, frequencies are multiples of 10.
  if(fHz % 10) return (fHz + 10 - (fHz % 10));

  return fHz;
}

// Yields a small gain in performance over sscanf and friends in CE's
// environment due to naive parsing rules and lack of locale awareness
// on our part.
static int extractInt(const char** string)
{
  assert(string && *string);

  int result = 0;
  bool isNegative = false;
  const char* c = *string;

  #define IS_PAYLOAD() ((*c >= '0' && *c <= '9') || *c == '-')

  // Eat anything that's not a digit.
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

  return (isNegative ? -result : result);
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

  static int samples[2000] = {0};

  static const size_t sampleCount = (sizeof samples / sizeof *samples);

  for(char lineBuffer[8192] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    int* sample = samples;

    // Fetch samples.
    for(const char* cursor = lineBuffer; *cursor;)
    {
      assert(sample < &samples[sizeof samples / sizeof *samples]);

      *sample++ = extractInt(&cursor);
    }

    // 20 kHz sampling rate, as per the problem statement.
    const unsigned fHz = getFrequency(samples, sampleCount, 2e4);

    // As per the problem statement.
    assert(!(fHz % 10) && (fHz >= 150) && (fHz <= 2000));

    printf("%u\n", fHz);
  }

  // The CRT takes care of cleanup.
}
