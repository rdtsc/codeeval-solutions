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

  for(char lineBuffer[256] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    char* marker = strchr(lineBuffer, '|');

    assert(marker);

    *(marker - 1) = '\0';
    *(marker + 1) = '\0';

    const char* lhs = lineBuffer,
              * rhs = (marker + 2);

    size_t mistakes = 0;

    while(*lhs) if(*lhs++ != *rhs++) ++mistakes;

    puts
    (
      mistakes == 0 ? "Done"    :
      mistakes <= 2 ? "Low"     :
      mistakes <= 4 ? "Medium"  :
      mistakes <= 6 ? "High"    :
                      "Critical"
    );
  }

  // The CRT takes care of cleanup.
}
