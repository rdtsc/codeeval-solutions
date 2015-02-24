#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[5120] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[384] = "", * line = lineBuffer;
      fgets(lineBuffer, sizeof lineBuffer, inputStream);
      line = lineBuffer)
  {
    size_t lineLength = strlen(line);

    // Trim trailing newline, if applicable.
    if(lineLength && line[lineLength - 1] == '\n') line[--lineLength] = '\0';

    if(lineLength <= 55) puts(line);

    else
    {
      // 40 chars is our text length limit.
      line[40] = '\0';

      // Find the last space...
      char* const lastSpace = strrchr(line, ' ');

      // ...and ignore everything after it (inclusive).
      if(lastSpace) *lastSpace = '\0';

      fputs(line, stdout);
      puts("... <Read More>");
    }
  }

  // The CRT takes care of cleanup.
}
