#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

static uint_fast64_t reverseDigits(uint_fast64_t n)
{
  uint_fast64_t result = 0;

  for(;n; n /= 10) result = (result * 10) + (n % 10);

  return result;
}

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
    uint_fast64_t m = n + reverseDigits(n), stepCount = 1;

    for(;m != reverseDigits(m); ++stepCount) m += reverseDigits(m);

    printf("%" PRIuFAST64 " %" PRIuFAST64 "\n", stepCount, m);
  }

  // The CRT takes care of cleanup.
}
