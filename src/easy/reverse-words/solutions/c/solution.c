#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

static void reverseString(char* const string)
{
  assert(string);

  for(char* a = string, * z = string + (strlen(string) - 1); z > a;)
  {
    const char oldA = *a;

    *a++ = *z;
    *z-- = oldA;
  }
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

  for(char lineBuffer[256] = "", * line = lineBuffer;
      fgets(line, sizeof lineBuffer, inputStream);
      line = lineBuffer)
  {
    // Reverse the input line: "abc def ghi" -> "ihg fed cba"
    reverseString(line);

    while(isspace(*line)) ++line;

    if(!*line) continue;

    for(char* word = strtok(line, " "); word;)
    {
      // Reconstruct each word: "ihg" -> "ghi"
      reverseString(word);

      fputs(word, stdout);

      if((word = strtok(NULL, " "))) putchar(' ');
    }

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
