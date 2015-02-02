#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

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

  typedef struct {double x, y;} Point;

  struct
  {
    double radius;
    Point center;
  } circle = {.radius = 0};

  Point point = {0, 0};

  #define POINT  "%*[^(](%lf,%lf)"
  #define RADIUS "%*[^:]:%lf"

  static const char* const scanFormat = POINT ";" RADIUS ";" POINT;

  #undef RADIUS
  #undef POINT

  #define CENTER &circle.center.x, &circle.center.y
  #define RADIUS &circle.radius
  #define POINT  &point.x, &point.y

  while(fscanf(inputStream, scanFormat, CENTER, RADIUS, POINT) == 5)
  {
    #undef POINT
    #undef RADIUS
    #undef CENTER

    // Work out the distance between the circle's center and the test point.
    const double pointDistance = sqrt(pow(point.x - circle.center.x, 2) +
                                      pow(point.y - circle.center.y, 2));

    // The test point lies within the circle if the distance between it and
    // the circle's center is less than the circle's radius.
    const bool isPointWithinCircle = (pointDistance < circle.radius);

    puts(isPointWithinCircle ? "true" : "false");
  }

  // The CRT takes care of cleanup.
}
