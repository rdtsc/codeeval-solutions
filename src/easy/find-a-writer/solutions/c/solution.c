#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

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

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    // Indices start after the pipe separator.
    char* c = strchr(lineBuffer, '|');

    assert(c);

    // Eat leading whitespace.
    for(++c; *c && *c == ' '; ++c);

    // Reuse the input buffer for output.
    char* result = c, * write = c;

    for(size_t index = 0;; ++c)
    {
      if(*c >= '0' && *c <= '9') index = (index * 10) + (*c - '0');

      // Term boundary.
      else if(*c == ' ' || !*c)
      {
        // Supplied indices are 1-based.
        assert(index);

        *write++ = lineBuffer[index - 1];
        index = 0;

        if(!*c)
        {
          *write = '\0';
          break;
        }
      }
    }

    puts(result);
  }

  // The CRT takes care of cleanup.
}
