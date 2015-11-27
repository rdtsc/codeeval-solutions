#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct {float x, y;} Point2d;

static bool pinOnSegment(const Point2d* const pin,
                         const Point2d* const a,
                         const Point2d* const b)
{
  assert(pin && a && b);

  const Point2d* const c = pin;

  const float ab = hypot((a->x - b->x), (a->y - b->y)),
              ac = hypot((a->x - c->x), (a->y - c->y)),
              bc = hypot((b->x - c->x), (b->y - c->y));

  // We're on the segment iff:
  // distance(self, A) + distance(self, B) == distance(A, B).
  return (fabs(ac + bc - ab) < FLT_EPSILON);
}

static bool pinInPolygon(const Point2d* const pin,
                         const Point2d* const outline,
                         const size_t pointCount)
{
  assert(pin && outline && pointCount);

  unsigned intersections = 0;

  // If a ray cast from our position intersects an odd number of polygon edges
  // then we are located within the polygon. Implemented as described in Q2.3
  // under 2D Polygon Computations in the comp.graphics.algorithms FAQ.
  for(size_t i = 0, j = (pointCount - 1); i < pointCount;j = i++)
  {
    // Points on the perimeter are considered to be inside the polygon.
    if(pinOnSegment(pin, &outline[i], &outline[j])) return true;

    const float x1 = outline[i].x, y1 = outline[i].y,
                x2 = outline[j].x, y2 = outline[j].y;

    const float deltaX = (x2 - x1),
                deltaY = (y2 - y1);

    if((y1 > pin->y) != (y2 > pin->y))
    {
      if(((deltaX / deltaY * (pin->y - y1)) + x1) > pin->x)
      {
        ++intersections;
      }
    }
  }

  return (intersections & 1);
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

  for(char lineBuffer[256] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    // Filter delimiters.
    for(char* c = lineBuffer; *c; ++c) if(!isdigit(*c)) *c = ' ';

    Point2d points[16] = {{.x = 0}};
    size_t pointCount = 0;

    // Unserialize points.
    {
      const char* cursor = lineBuffer;
      int bytesRead = 0;

      for(float x = 0, y = 0;
          sscanf(cursor, "%f%f%n", &x, &y, &bytesRead) == 2;
          cursor += bytesRead)
      {
        assert(pointCount < (sizeof points / sizeof *points));

        points[pointCount++] = (Point2d){x, y};
      }

      // As per the problem statement, including the test pin's coordinates.
      assert((pointCount > 3) && (pointCount <= 13));
    }

    const Point2d* const pin = &points[--pointCount];

    puts(pinInPolygon(pin, points, pointCount) ? "Prisoner" : "Citizen");
  }

  // The CRT takes care of cleanup.
}
