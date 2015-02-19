#include <assert.h>
#include <ctype.h>
#include <stddef.h>
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

  for(char lineBuffer[1024] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    size_t lineLength = strlen(lineBuffer);

    if(lineBuffer[lineLength - 1] == '\n') lineBuffer[--lineLength] = '\0';

    size_t count[2] = {0};
    enum {LOWERCASE, UPPERCASE};

    // Input lines are guaranteed to consist of only letters.
    for(const char* c = lineBuffer; *c; ++c) ++count[!islower(*c)];

    const float multiplier = lineLength * 0.01f;

    printf("lowercase: %.2f uppercase: %.2f\n", count[LOWERCASE] / multiplier,
                                                count[UPPERCASE] / multiplier);
  }

  // The CRT takes care of cleanup.
}
