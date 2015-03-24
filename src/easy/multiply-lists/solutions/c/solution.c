#include <assert.h>
#include <stddef.h>
#include <stdio.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[1536] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[128] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    unsigned lhs[16] = {0}, n = 0;
    size_t elementCount = 0;

    const char* c = lineBuffer;

    // Seek to the pipe separator while recording the terms on the left.
    for(;*c && *c != '|'; ++c)
    {
      if(*c >= '0' && *c <= '9') n = (n * 10) + (*c - '0');

      else if(*c == ' ')
      {
        assert(elementCount < sizeof lhs / sizeof *lhs);

        lhs[elementCount++] = n;
        n = 0;
      }
    }

    // Jump to the first digit belonging to the right list.
    while(*c && *c++ != ' ');

    // Multiply the items on the left with the items on the right.
    for(size_t i = 0;; ++c)
    {
      if(*c >= '0' && *c <= '9') n = (n * 10) + (*c - '0');

      else if(*c == ' ' || !*c || *c == '\n')
      {
        assert(i < elementCount);

        lhs[i++] *= n;
        n = 0;

        if(!*c || *c == '\n') break;
      }
    }

    for(size_t i = 0; i < elementCount; ++i) printf("%u ", lhs[i]);

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
