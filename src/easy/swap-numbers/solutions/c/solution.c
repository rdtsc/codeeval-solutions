#include <assert.h>
#include <ctype.h>
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

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    assert(isdigit(*lineBuffer));

    for(char* lhs = lineBuffer, * rhs = (lhs + 1); *rhs; ++rhs)
    {
      if(isdigit(*rhs))
      {
        const char digit = *rhs;

        *rhs = *lhs;
        *lhs = digit;

        // Probably EOL.
        if(*++rhs != ' ') break;

        lhs = ++rhs;

        assert(isdigit(*lhs));
      }
    }

    fputs(lineBuffer, stdout);
  }

  // The CRT takes care of cleanup.
}
