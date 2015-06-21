#include <assert.h>
#include <stddef.h>
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

  typedef unsigned value_type;
  typedef unsigned char count_type;

  static const char* const printFormat = "%u\n",
                   * const scanFormat  = "%u,%u,%u,%u,%u,%u";

  value_type n = 0, k = 0, a = 0, b = 0, c = 0, r = 0;

  while(fscanf(inputStream, scanFormat, &n, &k, &a, &b, &c, &r) == 6)
  {
    value_type values[1 << 10] = {0};
    count_type counts[1 << 16] = {0};

    static const size_t countsSize = (sizeof counts / sizeof *counts);

    // Reduce search space. Target array is cyclic with T <- (K + 1).
    n = (k + (n - k) % (k + 1));

    // Inject known values.
    {
      ++counts[*values = a];

      for(value_type i = 1; i < k; ++i)
      {
        ++counts[values[i] = (b * values[i - 1] + c) % r];
      }
    }

    // Inject unknown values.
    for(value_type i = k; i < n; ++i)
    {
      --counts[values[i - k]];

      for(value_type slot = 0; slot < countsSize; ++slot) if(!counts[slot])
      {
        ++counts[values[i] = slot];
        break;
      }
    }

    printf(printFormat, values[--n]);
  }

  // The CRT takes care of cleanup.
}
