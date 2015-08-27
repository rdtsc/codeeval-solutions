#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static bool containsAllFragments(const char* const chunk,
                                 const char* const fragments)
{
  assert(chunk && *chunk && fragments && *fragments);

  size_t frequencies[SCHAR_MAX + 1] = {0};

  #define AT(Char) frequencies[(size_t)(Char)]

  // Make candidate frequency map.
  for(const char* c = chunk; (*c && *c != '\n'); ++c)
  {
    assert(isalpha(*c));

    ++AT(*c);
  }

  // Attempt to remove fragments from the frequency map.
  for(const char* c = fragments; (*c && *c != '\n'); ++c)
  {
    if(!AT(*c)) return false;

    --AT(*c);
  }

  #undef AT

  return true;
}

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

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);
      putchar('\n'))
  {
    // <candidate>... <pipe> <fragments>
    char* fragments = strchr(lineBuffer, '|');

    assert(fragments);

    // Snip the string and fast-forward to the fragments section.
    for(*fragments++ = '\0'; *fragments == ' '; ++fragments);

    size_t matchCount = 0;

    // Process candidates.
    for(const char* chunk = strtok(lineBuffer, " ");
        chunk; chunk = strtok(NULL, " "))
    {
      if(containsAllFragments(chunk, fragments))
      {
        ++matchCount;

        fputs(chunk, stdout);
        putchar(' ');
      }
    }

    if(!matchCount) fputs("False", stdout);
  }

  // The CRT takes care of cleanup.
}
