#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[4096] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[1024] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    // First letter should be uppercased.
    bool shouldUppercase = true;

    for(char* c = lineBuffer; *c; ++c)
    {
      if(isalpha(*c))
      {
        *c = shouldUppercase ? toupper(*c) : tolower(*c);

        // Flip state for the next letter.
        shouldUppercase = !shouldUppercase;
      }

      else if(*c == '\n')
      {
        *c = '\0';
        break;
      }
    }

    puts(lineBuffer);
  }

  // The CRT takes care of cleanup.
}
