#include <assert.h>
#include <limits.h>
#include <stdio.h>

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

  for(char lineBuffer[128] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    unsigned shiftDistance  = UINT_MAX,
             minRowDistance = 0;

    // Fetch the smallest distance between neighboring X and Y cells on all
    // rows in the input board. This is the maximum number of steps we can
    // take before the X and Y shapes start touching, at least partially.
    for(const char* c = lineBuffer;; ++c)
    {
      if(*c == '.') ++minRowDistance;

      else if(*c == 'X' && *(c + 1) == 'Y')
      {
        shiftDistance = 0;
        break;
      }

      else if(*c == ',' || !*c || *c == '\n')
      {
        if(minRowDistance < shiftDistance) shiftDistance = minRowDistance;

        minRowDistance = 0;

        if(*c != ',') break;
      }
    }

    printf("%u\n", shiftDistance);
  }

  // The CRT takes care of cleanup.
}
