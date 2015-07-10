#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct {int x, y;} Point2d;

typedef struct {Point2d a, b, c;} Segment2d;

static inline bool isCollinear(const Point2d* const a,
                               const Point2d* const b,
                               const Point2d* const c)
{
  assert(a && b && c);

  #define COMPONENT(P1, P2, P3) ((P1)->x * ((P2)->y - (P3)->y))

  const int area = COMPONENT(a, b, c) +
                   COMPONENT(b, c, a) +
                   COMPONENT(c, a, b);

  return (area == 0);

  #undef COMPONENT
}

static inline bool isDuplicate(const Point2d* const a,
                               const Point2d* const b,
                               const Point2d* const c,
                               const Segment2d* const segment)
{
  assert(a && b && c && segment);

  const int lhs = (b->y - a->y) * (segment->b.x - segment->a.x),
            rhs = (b->x - a->x) * (segment->b.y - segment->a.y);

  const bool isSameSlope = (lhs == rhs);

  if(!isSameSlope) return false;

  #define IS_POINT_EQUAL(P1, P2) (((P1).x == (P2).x) && ((P1).y == (P2).y))

  #define IS_POINT_SHARED(Point) (IS_POINT_EQUAL(segment->a, (Point)) || \
                                  IS_POINT_EQUAL(segment->b, (Point)) || \
                                  IS_POINT_EQUAL(segment->c, (Point)))

  const bool hasSharedPoints = IS_POINT_SHARED(*a) ||
                               IS_POINT_SHARED(*b) ||
                               IS_POINT_SHARED(*c);

  return hasSharedPoints;

  #undef IS_POINT_SHARED
  #undef IS_POINT_EQUAL
}

static size_t getLineCount(const Point2d* const points,
                           const size_t pointCount)
{
  assert(points && pointCount);

  Segment2d segments[256] = {{.a = {0}}};
  size_t segmentCount = 0;

  for(size_t i = (0 + 0); i < pointCount; ++i)
  for(size_t j = (i + 1); j < pointCount; ++j)
  for(size_t k = (j + 1); k < pointCount; ++k)
  {
    if(isCollinear(&points[i], &points[j], &points[k]))
    {
      bool isNewSegment = true;

      for(size_t l = 0; l < segmentCount; ++l)
      {
        if(isDuplicate(&points[i], &points[j], &points[k], &segments[l]))
        {
          isNewSegment = false;
          break;
        }
      }

      if(isNewSegment)
      {
        assert(segmentCount < (sizeof segments / sizeof *segments));

        Segment2d* const segment = &segments[segmentCount++];

        segment->a = points[i];
        segment->b = points[j];
        segment->c = points[k];
      }
    }
  }

  return segmentCount;
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

  static char stdoutBuffer[256] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[1024] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    Point2d points[256] = {{0}};
    size_t pointCount = 0;

    for(const char* cursor = lineBuffer; *cursor;)
    {
      assert(pointCount < (sizeof points / sizeof *points));

      Point2d* const point = &points[pointCount++];

      point->x = extractInt(&cursor);
      point->y = extractInt(&cursor);
    }

    printf("%zu\n", getLineCount(points, pointCount));
  }

  // The CRT takes care of cleanup.
}
