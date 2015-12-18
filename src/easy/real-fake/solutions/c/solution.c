#include <assert.h>
#include <ctype.h>
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

  for(char lineBuffer[64] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    unsigned tally = 0;
    size_t digitIndex = 0;

    for(const char* c = lineBuffer; *c && *c != '\n'; ++c)
    {
      assert(isdigit(*c) || (*c == ' '));

      if(isdigit(*c)) tally += (*c - '0') * ((++digitIndex & 1) + 1);
    }

    puts(tally % 10 ? "Fake" : "Real");
  }

  // The CRT takes care of cleanup.
}
