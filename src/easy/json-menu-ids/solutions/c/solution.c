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

  static const char idKey[] = "\"id\":", labelKey[] = "\"label\":";

  for(char lineBuffer[1024] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    unsigned idSum = 0;

    // HACK: This is extremely fragile and ugly. I don't think we're expected to
    // implement an actual JSON parser for this challenge, though, so whatever.
    for(const char* chunk = strtok(lineBuffer, "{}");
        chunk; chunk = strtok(NULL, "{}"))
    {
      const char* idPos = strstr(chunk, idKey);

      // The chunk should have an ID as well as a label.
      if(idPos && strstr(chunk, labelKey))
      {
        unsigned id = 0;

        idPos += sizeof(idKey) - 1;

        while(*idPos == ' ') ++idPos;

        while(*idPos && *idPos >= '0' && *idPos <= '9')
        {
          id = (id * 10) + (*idPos++ - '0');
        }

        idSum += id;
      }
    }

    printf("%u\n", idSum);
  }

  // The CRT takes care of cleanup.
}
