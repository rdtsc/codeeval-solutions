#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
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

  for(char lineBuffer[256] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    if(!isdigit(*lineBuffer)) continue;

    unsigned terms[64] = {0}, targetSum = 0;
    size_t termCount = 0;

    // Fetch the terms and the target sum.
    {
      const char* c = lineBuffer;

      // Terms.
      for(unsigned n = 0; *c; ++c)
      {
        if(*c == ',' || *c == ';')
        {
          assert(n > terms[termCount]);

          terms[termCount++] = n;
          n = 0;

          if(*c == ';') break;
        }

        else
        {
          assert(isdigit(*c));

          n = (n * 10) + (*c - '0');
        }
      }

      // Target sum.
      for(++c; *c && *c != '\n'; ++c) targetSum = (targetSum * 10) + (*c - '0');
    }

    assert(targetSum);

    // We should bail early if the last two terms in the sorted sequence are
    // less than the target sum since there's no meaningful work to do.
    if(termCount >= 2 &&
      (terms[termCount - 1] + terms[termCount - 2]) < targetSum)
    {
      puts("NULL");
      continue;
    }

    bool foundPairs = false;

    for(size_t i = 0; i < termCount; ++i)
    {
      for(size_t j = i + 1; j < termCount; ++j)
      {
        const unsigned pairSum = terms[i] + terms[j];

        // Since the sequence is sorted, we can bail early if the current pair's
        // sum exceeds the target sum without checking the rest of the sequence.
        if(pairSum > targetSum) break;

        else if(pairSum == targetSum)
        {
          if(foundPairs) putchar(';');
          else if(!foundPairs) foundPairs = true;

          printf("%u,%u", terms[i], terms[j]);
          break;
        }
      }
    }

    if(!foundPairs) puts("NULL");
    else putchar('\n');
  }

  // The CRT takes care of cleanup.
}
