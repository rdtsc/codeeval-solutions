#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[4096] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    char wordBuffer  [sizeof lineBuffer / 2] = "", * words   = wordBuffer,
         numberBuffer[sizeof lineBuffer / 2] = "", * numbers = numberBuffer;

    // Classify and dump the elements into appropriate storage.
    for(const char* chunk = strtok(lineBuffer, ",\n");
        chunk; chunk = strtok(NULL, ",\n"))
    {
      #define RECORD(Type) do {                                         \
        assert((size_t)(Type##s - Type##Buffer) < sizeof Type##Buffer); \
                                                                        \
        const size_t bufferSize =                                       \
          sizeof Type##Buffer - (Type##s - Type##Buffer);               \
                                                                        \
        Type##s += snprintf(Type##s, bufferSize, "%s,", chunk);         \
      } while(false)

      if(*chunk >= '0' && *chunk <= '9') RECORD(number);

      else RECORD(word);

      #undef RECORD
    }

    const bool hasWords   = words   - wordBuffer,
               hasNumbers = numbers - numberBuffer;

    if(hasWords)
    {
      // Remove trailing comma.
      *(--words) = '\0';

      fputs(wordBuffer, stdout);
    }

    // The pipe separator should only be shown if both
    // types of data are present in the input.
    if(hasWords && hasNumbers) putchar('|');

    if(hasNumbers)
    {
      // Remove trailing comma.
      *(--numbers) = '\0';

      fputs(numberBuffer, stdout);
    }

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
