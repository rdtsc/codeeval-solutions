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

  static char stdoutBuffer[512] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[384] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    unsigned values[100] = {0};
    size_t valueCount = 0;

    // Yields a small gain in performance over sscanf and friends in CE's
    // environment due to naive parsing rules and lack of locale awareness on
    // our part.
    {
      unsigned value = 0;

      for(const char* c = lineBuffer;; ++c)
      {
        if(*c >= '0' && *c <= '9') value = (value * 10) + (*c - '0');

        else if(*c == ' ' || *c == '\n' || !*c)
        {
          assert(valueCount < sizeof values / sizeof *values);

          values[valueCount++] = value;
          value = 0;

          if(*c == '\n' || !*c) break;
        }
      }
    }

    // Input terms are guaranteed to fall within: [0,99]
    bool history[100] = {false};

    unsigned duplicates[sizeof values / sizeof *values] = {0};
    size_t duplicateCount = 0;

    for(size_t i = valueCount; i--;)
    {
      const unsigned value = values[i];

      assert(value < (sizeof history / sizeof *history));

      // Haven't seen this before.
      if(!history[value])
      {
        history[value] = true;
        continue;
      }

      bool isNewTerm = true;

      for(size_t j = 0; j < duplicateCount; ++j) if(duplicates[j] == value)
      {
        isNewTerm = false;
        break;
      }

      if(isNewTerm)
      {
        assert(duplicateCount < (sizeof duplicates / sizeof *duplicates));

        duplicates[duplicateCount++] = value;
      }
    }

    for(size_t i = duplicateCount; i--;) printf("%u ", duplicates[i]);

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
