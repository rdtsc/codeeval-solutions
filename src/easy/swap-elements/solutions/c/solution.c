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

  static char stdoutBuffer[8192] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    const char* c = lineBuffer;
    unsigned numbers[256] = {0};
    size_t numberCount = 0;

    // Get the number sequence before the colon. Yields a small gain in
    // performance over sscanf and friends in CE's environment due to naive
    // parsing rules and lack of locale awareness on our part.
    for(unsigned n = 0; *c; ++c)
    {
      if(*c >= '0' && *c <= '9') n = (n * 10) + (*c - '0');

      // New term.
      else if(*c == ' ')
      {
        numbers[numberCount++] = n;
        n = 0;
      }

      // End of sequence.
      else if(*c == ':')
      {
        ++c;
        break;
      }

      assert(numberCount < (sizeof numbers / sizeof *numbers));
    }

    // Get target indices after the colon and apply swapping operations.
    // Ditto regarding speed.
    for(unsigned positions[2] = {0}, * n = positions;; ++c)
    {
      if(*c >= '0' && *c <= '9') *n = ((*n) * 10) + (*c - '0');

      // Transition from LHS to RHS.
      else if(*c == '-') ++n;

      // Apply swap.
      else if(*c == ',' || *c == '\n' || !*c)
      {
        const unsigned temp   = numbers[positions[0]];
        numbers[positions[0]] = numbers[positions[1]];
        numbers[positions[1]] = temp;

        positions[0] = positions[1] = 0;
        n = positions;

        if(*c == '\n' || !*c) break;
      }

      assert((n - positions) < 2);
    }

    // Reuse the input buffer for increased performance. DIY formatting avoids
    // a bunch of printf calls.
    {
      char* out = lineBuffer;

      for(size_t i = 0; i < numberCount; ++i)
      {
        char digits[16] = "";

        #define THRESHOLD(Value) numbers[i] < (1e##Value) ? (Value)

        // Should be a bit faster than a call to log10.
        const size_t digitCount = THRESHOLD(1) : THRESHOLD(2) :
                                  THRESHOLD(3) : THRESHOLD(4) :
                                  THRESHOLD(5) : 0;

        assert(digitCount && digitCount < (sizeof digits / sizeof *digits));

        #undef THRESHOLD

        // Stuff N's digits into the `digits` array and re-inject them into
        // the input buffer.
        {
          unsigned n = numbers[i];

          for(size_t j = digitCount; j--; n /= 10) digits[j] = (n % 10) + '0';

          for(size_t j = 0; j < digitCount; ++j)
          {
            *out++ = digits[j];

            assert((size_t)(out - lineBuffer) < sizeof lineBuffer);
          }

          *out++ = ' ';
        }
      }

      *out = '\0';
    }

    puts(lineBuffer);
  }

  // The CRT takes care of cleanup.
}
