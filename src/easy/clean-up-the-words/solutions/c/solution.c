#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[2048] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    // Filter garbage.
    for(char* c = lineBuffer; *c; ++c)
    {
      if(!isalpha(*c)) *c = ' ';

      else *c = tolower(*c);
    }

    // Explode words.
    for(const char* word = strtok(lineBuffer, " ");
        word; word = strtok(NULL, " "))
    {
      fputs(word, stdout);
      putchar(' ');
    }

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
