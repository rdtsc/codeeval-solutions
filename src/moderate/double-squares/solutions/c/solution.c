#include <assert.h>
#include <math.h>
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

  unsigned n = 0;

  for(fscanf(inputStream, "%u", &n); fscanf(inputStream, "%u", &n) == 1;)
  {
    const unsigned upperLimit = sqrt(n);

    unsigned result = 0;

    for(unsigned i = 0; i <= upperLimit; ++i)
    {
      const unsigned value = (n - i * i),
                     test  = sqrt(value);

      if(test < i) break;

      else if(test * test == value) ++result;
    }

    printf("%u\n", result);
  }

  // The CRT takes care of cleanup.
}
