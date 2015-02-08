#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
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

  for(uint_least32_t n = 0; fscanf(inputStream, "%" SCNuLEAST32, &n) == 1;)
  {
    if(!n) puts("0");

    else
    {
      bool skippedPadding = false;

      for(uint_least32_t i = sizeof n * CHAR_BIT; i--;)
      {
        const bool bit = (n >> i) & 1;

        // Start printing the digits only after we've passed the first set bit.
        if(bit && !skippedPadding) skippedPadding = true;
        if(skippedPadding) putchar('0' + bit);
      }

      putchar('\n');
    }
  }

  // The CRT takes care of cleanup.
}
