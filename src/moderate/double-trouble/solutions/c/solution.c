#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

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

  for(char lineBuffer[128] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    // Kill trailing newline.
    {
      char* const cursor = strchr(lineBuffer, '\n');

      if(cursor) *cursor = '\0';
    }

    const size_t dataLength = strlen(lineBuffer);

    assert(!(dataLength & 1));

    const char* const dataLeft  = lineBuffer,
              * const dataRight = &lineBuffer[dataLength / 2];

    const char* lhs = dataLeft,
              * rhs = dataRight;

    bool isValidInput = true;
    unsigned asteriskCount = 0;

    for(;lhs != dataRight; ++lhs, ++rhs)
    {
      // Wildcard pair.
      if((*lhs == '*') && (*rhs == '*')) ++asteriskCount;

      // Complete mismatch between letters.
      else if((*lhs != *rhs) && !(*lhs == '*' || *rhs == '*'))
      {
        isValidInput = false;
        break;
      }
    }

    if(!isValidInput) puts("0");

    else
    {
      // |{A, B}| = 2.
      const uint_least64_t variantCount = pow(2, asteriskCount);

      printf("%" PRIuLEAST64 "\n", variantCount);
    }
  }

  // The CRT takes care of cleanup.
}
