#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

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

  for(char lineBuffer[12288] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    // N falls within the range of [0, 100].
    unsigned count[101] = {0};
    size_t termCount = 0;

    for(const char* chunk = lineBuffer; *chunk; ++termCount)
    {
      size_t term = 0;

      // Reconstruct the term.
      for(const char* c = chunk; *c >= '0' && *c <= '9'; ++c)
      {
        // Eat this digit.
        ++chunk;

        term = (term * 10) + (*c - '0');
      }

      ++count[term];

      // Eat any term separators.
      while(*chunk && *chunk++ != ',');
    }

    bool hasMajorElement = false;

    // "Major" elements appear more than L/2 times in the sequence of terms.
    for(size_t i = 0; i < sizeof count / sizeof *count; ++i)
    {
      if(count[i] > (termCount / 2))
      {
        printf("%zu\n", i);
        hasMajorElement = true;
        break;
      }
    }

    if(!hasMajorElement) puts("None");
  }

  // The CRT takes care of cleanup.
}
