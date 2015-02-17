#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

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

  static const unsigned romanBaseLut[24] =
  {
    #ifdef B
    #error
    #endif

    #define B(Literal) [(Literal) - 'A']

    B('I') = 1,  B('V') = 5,   B('X') = 10,
    B('L') = 50, B('C') = 100, B('D') = 500,
    B('M') = 1000

    #undef B
  };

  for(char lineBuffer[64] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    const char* c = lineBuffer + strlen(lineBuffer) - 1;

    if(*c == '\n') --c;

    assert(!((c - lineBuffer + 1) & 1));

    int_fast64_t value = 0;

    for(unsigned oldBase = 1; c >= lineBuffer; c -= 2)
    {
      const unsigned r  = romanBaseLut[*c - 'A'], // Roman base.
                     a  = *(c - 1) - '0',         // Arabic digit.
                     ra = r * a;                  // Aromatic pair.

      value = (oldBase > r) ? (value - ra) : (value + ra);

      oldBase = r;
    }

    printf("%" PRIdFAST64 "\n", value);
  }

  // The CRT takes care of cleanup.
}
