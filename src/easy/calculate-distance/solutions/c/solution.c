#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[512] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  struct {int x, y;} a = {0}, b = {0};

  while(fscanf(inputStream, "(%d, %d) (%d, %d) ", &a.x, &a.y,
                                                  &b.x, &b.y) == 4)
  {
    // Distance formula.
    const unsigned distance = sqrt(pow(b.x - a.x, 2) +
                                   pow(b.y - a.y, 2));

    printf("%u\n", distance);
  }

  // The CRT takes care of cleanup.
}
