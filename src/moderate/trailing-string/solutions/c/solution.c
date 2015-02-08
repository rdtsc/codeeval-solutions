#include <assert.h>
#include <stddef.h>
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

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    if(*lineBuffer == '\n') continue;

    char* rhs = lineBuffer + strlen(lineBuffer) - 1;
    size_t rhsLength = 0;

    // Jump to the begining of the second string.
    for(;rhs >= lineBuffer && *rhs != ','; --rhs)
    {
      if(*rhs == '\n') *rhs = '\0';
      else ++rhsLength;
    }

    // Kill the LHS/RHS comma separator.
    *rhs++ = '\0';

    const size_t lhsLength = rhs - lineBuffer - 1;

    if(rhsLength > lhsLength) puts("0");

    else
    {
      const char* const lhs = lineBuffer + (lhsLength - rhsLength);

      puts(strcmp(lhs, rhs) ? "0" : "1");
    }
  }

  // The CRT takes care of cleanup.
}
