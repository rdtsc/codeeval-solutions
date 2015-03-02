#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>

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

  for(char lineBuffer[16] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    char digitOccurances[10] = {0};
    size_t digitCount = 0;

    // Build the digit frequency map.
    for(char* c = lineBuffer; *c && *c != '\n'; ++c, ++digitCount)
    {
      assert(isdigit(*c));

      ++digitOccurances[(size_t)(*c -= '0')];
    }

    for(size_t i = 0; i < digitCount; ++i)
    {
      // Digit position should correspond to the digit count.
      if(digitOccurances[i] != lineBuffer[i])
      {
        *lineBuffer = '\0';
        break;
      }
    }

    puts(*lineBuffer ? "1" : "0");
  }

  // The CRT takes care of cleanup.
}
