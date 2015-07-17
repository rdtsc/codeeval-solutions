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

  for(char lineBuffer[128] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    // [A-Z] + [a-z]
    char hitCount[52] = "";

    // Count the number of occurrences of each character.
    for(const char* c = lineBuffer; *c && *c != '\n'; ++c)
    {
      if(islower(*c))      ++hitCount[*c - 'a'];
      else if(isupper(*c)) ++hitCount[*c - 'A'];
    }

    // Dump the first character that's unique.
    for(const char* c = lineBuffer; *c && *c != '\n'; ++c) if(isalpha(*c))
    {
      const size_t probe = *c - (islower(*c) ? 'a' : 'A');

      if(hitCount[probe] == 1)
      {
        putchar(*c);
        putchar('\n');
        break;
      }
    }
  }

  // The CRT takes care of cleanup.
}
