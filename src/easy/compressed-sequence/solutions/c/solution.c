#include <assert.h>
#include <stdio.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[2048] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[512] = "", outputBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    const char* c = lineBuffer;
    char* output = outputBuffer;

    // Yields a small gain in performance over sscanf in CE's environment due
    // to naive parsing rules and lack of locale awareness on our part.
    #define READ_NEXT(V) do {                                                  \
      while(*c && *c == ' ') ++c;                                              \
      for((V) = 0; *c >= '0' && *c <= '9'; ++c) (V) = ((V) * 10) + (*c - '0'); \
    } while(0)

    unsigned term = 0, termCount = 1;

    READ_NEXT(term);

    for(unsigned termPrev = term; *c && *c != '\n'; ++c)
    {
      READ_NEXT(term);

      // Series being continued.
      if(term == termPrev) ++termCount;

      // Series interrupted.
      else
      {
        // HACK: The macros below yield a slightly higher score in CE's
        // environment than a bunch of [s]printf calls due to some assumptions
        // about the format of the output.
        #define EMIT(Char) (*output++ = (Char))

        #define DUMP(Value) do {                                             \
          if((Value) / 100) EMIT('0' + ((Value) / 100) % 10);                \
          if((Value) / 10 || (Value) / 100) EMIT('0' + ((Value) / 10) % 10); \
          EMIT('0' + (Value) % 10);                                          \
        } while(0)

        #define DUMP_PAIR(Left, Right) do {       \
          DUMP((Left)); EMIT(' '); DUMP((Right)); \
        } while(0)

        DUMP_PAIR(termCount, termPrev); EMIT(' ');

        termPrev  = term;
        termCount = 1;
      }
    }

    // Account for the last set.
    DUMP_PAIR(termCount, term);

    #undef EMIT
    #undef DUMP
    #undef DUMP_PAIR
    #undef READ_NEXT

    *output = '\0';
    puts(outputBuffer);
  }

  // The CRT takes care of cleanup.
}
