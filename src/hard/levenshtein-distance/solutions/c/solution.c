#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
  const char* const data;
  unsigned connections;
} Word;

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

  // Pre-computed by the C++ version of this solution via -DGENERATE_LUT.
  static const Word lut[] =
  {
    #include "lut.dat"
  };

  // Breakpoints based on the position of the first occurrence of each letter
  // in the sorted LUT.
  static const size_t breakpoints[26] =
  {
    #define SEGMENT(Char, Value) [(Char) - 'a'] = (Value)

    SEGMENT('a',     0), SEGMENT('c',   672),
    SEGMENT('e',   872), SEGMENT('f',  1113),
    SEGMENT('g',  1287), SEGMENT('h',  1355),
    SEGMENT('j',  1628), SEGMENT('k',  2101),
    SEGMENT('l',  2790), SEGMENT('m',  3477),
    SEGMENT('n',  4167), SEGMENT('o',  4856),
    SEGMENT('p',  5544), SEGMENT('q',  6253),
    SEGMENT('r',  6424), SEGMENT('s',  7133),
    SEGMENT('t',  7842), SEGMENT('u',  8551),
    SEGMENT('v',  9239), SEGMENT('w',  9928),
    SEGMENT('y', 10626), SEGMENT('z', 11063)

    #undef SEGMENT
  };

  static const size_t lutSize = (sizeof lut / sizeof *lut);

  static const char inputTerminator[] = "END OF";

  for(char lineBuffer[128] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    if(!strncmp(lineBuffer, inputTerminator, (sizeof inputTerminator - 1)))
    {
      break;
    }

    // Snip trailing newlines.
    for(char* c = lineBuffer; *c; ++c) if(*c == '\n') *c = '\0';

    assert(isalpha(*lineBuffer) && islower(*lineBuffer));

    // Linear search is fast enough if we jump to the relevant segment first.
    for(size_t i = breakpoints[*lineBuffer - 'a']; i < lutSize; ++i)
    {
      if(!strcmp(lineBuffer, lut[i].data))
      {
        printf("%u\n", lut[i].connections);
        break;
      }
    }
  }

  // The CRT takes care of cleanup.
}
