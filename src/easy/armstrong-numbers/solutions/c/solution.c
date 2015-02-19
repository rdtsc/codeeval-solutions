#include <assert.h>
#include <inttypes.h>
#include <math.h>
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

  for(uint_fast64_t n = 0; fscanf(inputStream, "%" SCNuFAST64, &n) == 1;)
  {
    const unsigned digitCount = log10(n) + 1;

    uint_fast64_t armstrongSum = 0;

    for(uint_fast64_t i = n; i; i /= 10)
    {
      armstrongSum += pow(i % 10, digitCount);
    }

    puts(armstrongSum == n ? "True" : "False");
  }

  // The CRT takes care of cleanup.
}
