#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[768] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[64] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    char* pattern = strchr(lineBuffer, ' ');

    assert(pattern);

    bool shouldAdd = false;
    int_fast32_t numbers[2] = {0};
    size_t i = 0;

    // Kill the space separator and extract the numbers based on the pattern.
    for(*pattern++ = '\0'; *pattern && *pattern != '\n'; ++pattern)
    {
      const char c = *pattern;

      // Lowercase letters map to digits.
      if(c >= 'a' && c <= 'z')
      {
        numbers[i] = (numbers[i] * 10) + (lineBuffer[c - 'a'] - '0');
      }

      else
      {
        ++i;

        assert(i < 2);

        if(c == '+') shouldAdd = true;
      }
    }

    // Expecting at most one operator in the pattern.
    assert(i == 1);

    printf("%" PRIdFAST32 "\n", (shouldAdd) ? (numbers[0] + numbers[1]) :
                                              (numbers[0] - numbers[1]));
  }

  // The CRT takes care of cleanup.
}
