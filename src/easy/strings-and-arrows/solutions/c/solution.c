#include <assert.h>
#include <stdio.h>
#include <string.h>

static unsigned getSubstringCount(const char* source,
                                  const char* const target)
{
  assert(source && target);

  unsigned result = 0;

  for(source = strstr(source, target); source; ++result)
  {
    source = strstr(++source, target);
  }

  return result;
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

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    const unsigned arrowCount = getSubstringCount(lineBuffer, "<--<<") +
                                getSubstringCount(lineBuffer, ">>-->");

    printf("%u\n", arrowCount);
  }

  // The CRT takes care of cleanup.
}
