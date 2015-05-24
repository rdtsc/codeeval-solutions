#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static inline int strictWeakOrdering(const void* const lhs,
                                     const void* const rhs)
{
  assert(lhs && rhs);

  const double x = *(double*)lhs,
               y = *(double*)rhs;

  return (x < y) ? (-1) : (x > y);
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

  struct {int x, y;} points[4] = {{0}};

  static const char* scanFormat = "(%d,%d), (%d,%d), (%d,%d), (%d,%d) ";

  #define PT(Index) &points[(Index)].x, &points[(Index)].y

  while(fscanf(inputStream, scanFormat, PT(0), PT(1), PT(2), PT(3)) == 8)
  {
    #undef PT

    double distances[] =
    {
      #define DISTANCE(A, B) sqrt(                                          \
        (points[(B)].x - points[(A)].x) * (points[(B)].x - points[(A)].x) + \
        (points[(B)].y - points[(A)].y) * (points[(B)].y - points[(A)].y)   \
      )

      /* A */ DISTANCE(0, 1), // ┌──A──┐  ┌──B──┐  ┌──C──┐
      /* A */ DISTANCE(0, 2), // │0---1│  │0   1│  │0   1│
      /* A */ DISTANCE(0, 3), // │| \  │  │  / |│  │     │
      /* B */ DISTANCE(1, 2), // │|  \ │  │ /  |│  │     │
      /* B */ DISTANCE(1, 3), // │2   3│  │2   3│  │2---3│
      /* C */ DISTANCE(2, 3)  // └─────┘  └─────┘  └─────┘

      /*
        - Faces:       {(0,1), (0,2), (1,3), (2,3)}
        - Hypotenuses: {(0,3), (1,2)}
      */

      #undef DISTANCE
    };

    static const size_t distanceCount = (sizeof distances / sizeof *distances);

    assert(distanceCount == 6);

    // Group face lengths and hypotenuses.
    qsort(distances, distanceCount, sizeof *distances, strictWeakOrdering);

    // Hypotenuses should be identical.
    bool isSquare = (distances[4] == distances[5]);

    // Face lengths should be identical.
    if(isSquare) for(size_t i = 0; i < (distanceCount - 2); ++i)
    {
      if(!distances[i] || distances[i] != *distances)
      {
        isSquare = false;
        break;
      }
    }

    puts(isSquare ? "true" : "false");
  }

  // The CRT takes care of cleanup.
}
