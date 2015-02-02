#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[4096] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(unsigned x = 0, y = 0, n = 0;
      fscanf(inputStream, "%u %u %u", &x, &y, &n) == 3;)
  {
    for(unsigned i = 1; i <= n; ++i)
    {
      const bool isF = !(i % x), // 'F' for numbers divisible by X.
                 isB = !(i % y); // 'B' for numbers divisible by Y.

      if(isF && isB) fputs("FB", stdout);
      else if(isF)   putchar('F');
      else if(isB)   putchar('B');
      else           printf("%u", i);

      // Looks like CE doesn't care about trailing spaces. Whatever.
      putchar(' ');
    }

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
