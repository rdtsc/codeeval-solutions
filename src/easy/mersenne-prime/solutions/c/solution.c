#include <assert.h>
#include <stddef.h>
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

  static const unsigned terms[] = {3, 7, 31, 127, 2047};

  for(unsigned n = 0; fscanf(inputStream, "%u", &n) == 1; putchar('\n'))
  {
    printf("%u", *terms);

    for(size_t i = 1; i < (sizeof terms / sizeof *terms); ++i)
    {
      if(terms[i] >= n) break;

      printf(", %u", terms[i]);
    }
  }

  // The CRT takes care of cleanup.
}
