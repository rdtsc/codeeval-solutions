#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static inline int descendingOrdering(const void* const lhs,
                                     const void* const rhs)
{
  assert(lhs && rhs);

  register const int x = *(int*)lhs,
                     y = *(int*)rhs;

  return (x > y) ? (-1) : (x < y);
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[2560] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[256] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    int items[48] = {0};
    size_t itemCount = 0;

    // Dealing with fixed-point numbers. Yields a small gain in performance
    // over sscanf/atof in CE's environment due to naive parsing rules and
    // lack of locale awareness on our part.
    {
      int n = 0;
      bool isNegative = false;

      for(const char* c = lineBuffer;; ++c)
      {
        if(*c == ' ' || *c == '\n' || !*c)
        {
          assert(itemCount < sizeof items / sizeof *items);

          if(isNegative) n = -n;

          items[itemCount++] = n;

          isNegative = false;
          n = 0;

          if(*c == '\n' || !*c) break;
        }

        else if(*c >= '0' && *c <= '9') n = (n * 10) + (*c - '0');

        else if(*c == '-') isNegative = true;
      }
    }

    qsort(items, itemCount, sizeof *items, descendingOrdering);

    // Reuse the input buffer for our output.
    char* output = lineBuffer + sizeof lineBuffer - 1;

    for(size_t i = 0; i < itemCount; ++i)
    {
      const int item = abs(items[i]);
      const bool isNegative = (items[i] < 0);
      bool injectedPeriod = false;

      #define EMIT(Char) (*--output = (Char))

      // Reconstruct the numbers RTL. ~3x gain in performance in CE's
      // environment over printf-ing.
      for(int n = item, digitIndex = 0; n; n /= 10)
      {
        // We're supposed to be printing: "%.3f"
        if(digitIndex++ == 3)
        {
          EMIT('.');
          injectedPeriod = true;
        }

        EMIT((n % 10) + '0');
      }

      // Take care of padding for values under 1k.
      if(!injectedPeriod)
      {
        if(item <   1) EMIT('0');
        if(item <  10) EMIT('0');
        if(item < 100) EMIT('0');

        EMIT('.');

        if(item < 1000) EMIT('0');
      }

      if(isNegative) EMIT('-');

      EMIT(' ');

      #undef EMIT

      assert(output >= lineBuffer);
    }

    // Eat the leading space.
    puts(output + 1);
  }

  // The CRT takes care of cleanup.
}
