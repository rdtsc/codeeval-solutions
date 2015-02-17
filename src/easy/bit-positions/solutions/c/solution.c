#include <assert.h>
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

  for(unsigned n = 0, p = 0, q = 0;
      fscanf(inputStream, "%u,%u,%u", &n, &p, &q) == 3;)
  {
    const bool pBit = (n >> (p - 1)) & 1,
               qBit = (n >> (q - 1)) & 1;

    puts(pBit == qBit ? "true" : "false");
  }

  // The CRT takes care of cleanup.
}
