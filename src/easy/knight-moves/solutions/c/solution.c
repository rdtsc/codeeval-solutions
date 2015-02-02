#include <assert.h>
#include <stdio.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[1024] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char position[3] = ""; fscanf(inputStream, "%2s", position) == 1;)
  {
    const char c = position[0];
    const unsigned n = position[1] - '0';

    assert(c >= 'a' && c <= 'h');
    assert(n >= 1 && n <= 8);

    /*
      Possible knight position offsets.
      Could just use a LUT for all 64 starting positions, instead.

      5   @   @
      4 @       @
      3     N
      2 @       @
      1   @   @
        A B C D E
    */

    #define DUMP(x, y) printf("%c%u ", (c x), (n y))

    if(c >= 'c')
    {
      if(n >= 2) DUMP(-2, -1); // A2
      if(n <= 7) DUMP(-2, +1); // A4
    }

    if(c >= 'b')
    {
      if(n >= 3) DUMP(-1, -2); // B1
      if(n <= 6) DUMP(-1, +2); // B5
    }

    if(c <= 'g')
    {
      if(n >= 3) DUMP(+1, -2); // D1
      if(n <= 6) DUMP(+1, +2); // D5
    }

    if(c <= 'f')
    {
      if(n >= 2) DUMP(+2, -1); // E2
      if(n <= 7) DUMP(+2, +1); // E4
    }

    #undef DUMP

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
