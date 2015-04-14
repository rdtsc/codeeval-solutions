#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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

  struct {unsigned h, m, s;} t[2] = {{0}};

  // The two supplied timestamps are in ISO 8601/Extended format.
  static const char* const scanFormat = "%u:%u:%u %u:%u:%u";

  while(fscanf(inputStream, scanFormat, &t[0].h, &t[0].m, &t[0].s,
                                        &t[1].h, &t[1].m, &t[1].s) == 6)
  {
    const int lhsSeconds = ((t[0].h * 3600) + (t[0].m * 60) + t[0].s),
              rhsSeconds = ((t[1].h * 3600) + (t[1].m * 60) + t[1].s);

    const unsigned deltaSeconds = abs(lhsSeconds - rhsSeconds);

    printf("%02u:%02u:%02u\n", (deltaSeconds / 3600),    // HH
                               (deltaSeconds / 60) % 60, // MM
                               (deltaSeconds % 60));     // SS
  }

  // The CRT takes care of cleanup.
}
