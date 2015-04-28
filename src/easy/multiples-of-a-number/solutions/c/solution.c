#include <assert.h>
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

  for(unsigned x = 0, n = 0; fscanf(inputStream, "%u,%u", &x, &n) == 2;)
  {
    unsigned exponent = 1;

    // Get N's exponent; N = 2^exponent
    for(unsigned m = n; m > 2; m >>= 1) ++exponent;

    // Divide X by N.
    unsigned multiple = (x >> exponent);

    // Was the division clean?
    if(multiple * n != x) ++multiple;

    printf("%u\n", n * multiple);
  }

  // The CRT takes care of cleanup.
}
