#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[128] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    const char* source = lineBuffer + strlen(lineBuffer) - 1;

    if(*source == '\n') --source;

    // Our target is the last character in the string.
    char target = *source--;

    for(--source; source >= lineBuffer; --source) if(*source == target)
    {
      // Unset the target to indicate that we've found it in the source string.
      target = '\0';

      printf("%zu\n", source - lineBuffer);
      break;
    }

    if(target != '\0') puts("-1");
  }

  // The CRT takes care of cleanup.
}
