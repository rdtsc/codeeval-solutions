#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct
{
  /*
    (x1, y1) +------------+
             |            |
             +------------+ (x2, y2)
  */
  int x1, y1, x2, y2;
} Rectangle;

static inline bool hasOverlap(const Rectangle* const b,
                              const Rectangle* const a)
{
  assert(a && b);

  const bool noOverlap = a->x1 > b->x2 ||
                         a->x2 < b->x1 ||
                         a->y1 < b->y2 ||
                         a->y2 > b->y1;

  return !noOverlap;
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

  Rectangle a = {0}, b = {0};

  #define RECT_FORMAT "%d,%d,%d,%d"
  #define INPUT_FORMAT RECT_FORMAT "," RECT_FORMAT

  while(fscanf(inputStream, INPUT_FORMAT, &a.x1, &a.y1, &a.x2, &a.y2,
                                          &b.x1, &b.y1, &b.x2, &b.y2) == 8)
  {
    puts(hasOverlap(&a, &b) ? "True" : "False");
  }

  #undef INPUT_FORMAT
  #undef RECT_FORMAT

  // The CRT takes care of cleanup.
}
