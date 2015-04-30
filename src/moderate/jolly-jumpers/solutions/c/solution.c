#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  for(char lineBuffer[1024] = "", * c = lineBuffer;
      fgets(lineBuffer, sizeof lineBuffer, inputStream);
      c = lineBuffer)
  {
    size_t termCount = 0;

    // Get term count.
    {
      while(*c && *c >= '0' && *c <= '9')
      {
        termCount = (termCount * 10) + (*c++ - '0');
      }

      while(*c && *c == ' ') ++c;
    }

    // Sanity check. Unrestricted VLAs can lead to UB if allocation fails.
    assert(termCount && termCount <= 512);

    if(termCount == 1)
    {
      puts("Jolly");
      continue;
    }

    // VLAs yield a slightly better memory score on CodeEval than straight
    // [m,c]alloc. Shouldn't rely on this in C11 and onwards, though.
    int terms[termCount];
    memset(terms, 0, sizeof terms);

    // Get the payload. Yields a small gain in performance over sscanf and
    // friends in CE's environment due to naive parsing rules and lack of
    // locale awareness on our part.
    {
      int term = 0;
      bool isNegative = false;

      for(size_t termIndex = 0;; ++c)
      {
        if(*c == '-') isNegative = true;

        else if(*c >= '0' && *c <= '9') term = (term * 10) + (*c - '0');

        else if(*c == ' ' || *c == '\n' || !*c)
        {
          assert(termIndex < termCount);

          terms[termIndex++] = (isNegative ? -term : term);
          isNegative = false;
          term = 0;

          if(*c == '\n' || !*c) break;
        }
      }
    }

    bool history[termCount], isJollyJumper = true;
    memset(history, false, sizeof history);

    for(size_t i = 0; i < termCount - 1; ++i)
    {
      // Absolute difference between neighboring terms.
      const int delta = abs(terms[i] - terms[i + 1]);

      // The differences between successive elements should take on all
      // possible values in: [1, n).
      const bool isOutOfBounds = (!delta || (size_t)delta >= termCount);

      if(isOutOfBounds || history[delta - 1])
      {
        isJollyJumper = false;
        break;
      }

      history[delta - 1] = true;
    }

    puts(isJollyJumper ? "Jolly" : "Not jolly");
  }

  // The CRT takes care of cleanup.
}
