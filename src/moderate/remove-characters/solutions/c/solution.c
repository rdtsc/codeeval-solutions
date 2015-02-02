#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    const char* const sourceChunk = strtok(lineBuffer, ","),
              * const filterChunk = strtok(NULL, " \n");

    assert(source && filter);

    for(const char* source = sourceChunk; *source; ++source)
    {
      bool skipChar = false;

      for(const char* filter = filterChunk; *filter; ++filter)
      {
        if(*source == *filter)
        {
          skipChar = true;
          break;
        }
      }

      if(!skipChar) putchar(*source);
    }

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
