#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static char wordToDigit(const char* const word)
{
  assert(strlen(word) >= 3);

  switch(*word)
  {
    case 'z': return 0; // [z]ero
    case 'o': return 1; // [o]ne
    case 'e': return 8; // [e]ight
    case 'n': return 9; // [n]ine

    default: switch(word[1])
    {
      case 'w': return 2; // t[w]o
      case 'h': return 3; // t[h]ree
      case 'o': return 4; // f[o]ur
      case 'i': return (word[2] == 'x') + 5; // f[i]ve || s[i]x
      case 'e': return 7; // s[e]ven
    }
  }

  return CHAR_MAX;
}

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

  for(char lineBuffer[256] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    size_t resultIndex = 0;

    for(const char* chunk = strtok(lineBuffer, ";"); chunk;
        chunk = strtok(NULL, ";\n"))
    {
      lineBuffer[resultIndex++] = '0' + wordToDigit(chunk);
    }

    lineBuffer[resultIndex] = '\0';

    puts(lineBuffer);
  }

  // The CRT takes care of cleanup.
}
