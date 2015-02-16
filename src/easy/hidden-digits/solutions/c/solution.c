#include <assert.h>
#include <stddef.h>
#include <stdio.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[512] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[256] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    size_t bufferTail = 0;

    for(const char* c = lineBuffer; *c && *c != '\n'; ++c)
    {
      if((unsigned)*c - '0' <= 9U)
      {
        lineBuffer[bufferTail++] = *c;
      }

      // Map [a, j] -> [0, 9].
      else if((unsigned)*c - 'a' <= 9U)
      {
        lineBuffer[bufferTail++] = '0' + (*c - 'a');
      }
    }

    lineBuffer[bufferTail] = '\0';

    puts(bufferTail ? lineBuffer : "NONE");
  }

  // The CRT takes care of cleanup.
}
