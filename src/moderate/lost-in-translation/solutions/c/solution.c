#include <assert.h>
#include <stdio.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[2048] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  // The example in the problem statement explicitly covers all characters but
  // one. The following was quickly derived by hand and maps scrambled alphabet
  // positions to their corresponding plain-text characters.
  static const char* const mapping = "yhesocvxduiglbkrztnwjpfmaq";

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    for(char* c = lineBuffer; *c; ++c)
    {
      if(*c >= 'a' && *c <= 'z') *c = mapping[*c - 'a'];
    }

    fputs(lineBuffer, stdout);
  }

  // The CRT takes care of cleanup.
}
