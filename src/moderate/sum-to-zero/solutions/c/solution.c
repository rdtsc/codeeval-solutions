#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
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

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    int terms[256] = {0};
    size_t termCount = 0;

    // Yields a small gain in performance over sscanf and friends in CE's
    // environment due to naive parsing rules and lack of locale awareness on
    // our part.
    {
      int n = 0;
      bool isNegative = false;

      for(const char* c = lineBuffer;; ++c)
      {
        if(*c == ',' || *c == '\n' || !*c)
        {
          assert(termCount < sizeof terms / sizeof *terms);

          terms[termCount++] = (isNegative ? -n : n);

          isNegative = false;
          n = 0;

          if(*c == '\n' || !*c) break;
        }

        else if(*c >= '0' && *c <= '9') n = (n * 10) + (*c - '0');

        else if(*c == '-') isNegative = true;
      }
    }

    assert(termCount >= 4);

    unsigned result = 0;

    // Only dealing with 4 terms at a time, so this should be OK.
    for(size_t a = 0 + 0; a < termCount - 3; ++a)
    for(size_t b = a + 1; b < termCount - 2; ++b)
    for(size_t c = b + 1; c < termCount - 1; ++c)
    for(size_t d = c + 1; d < termCount - 0; ++d)
    {
      result += !(terms[a] + terms[b] + terms[c] + terms[d]);
    }

    printf("%u\n", result);
  }

  // The CRT takes care of cleanup.
}
