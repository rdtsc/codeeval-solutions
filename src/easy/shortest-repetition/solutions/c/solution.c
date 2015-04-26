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

  static char stdoutBuffer[128] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  static const bool isPrime[100] =
  {
    #define P(Index) [(Index)] = true

    P(02), P(03), P(05), P(07), P(11),
    P(13), P(17), P(19), P(23), P(29),
    P(31), P(37), P(41), P(43), P(47),
    P(53), P(59), P(61), P(67), P(71),
    P(73), P(79), P(83), P(89), P(97)

    #undef P
  };

  for(char lineBuffer[128] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    size_t lineLength = 0;
    bool isHomogeneous = true;

    // Get line length (less trailing newline) and check if the input consists
    // of a single repeating character.
    {
      char neighbor = *lineBuffer;

      for(const char* c = lineBuffer; *c && *c != '\n'; ++c)
      {
        if(neighbor != *c) isHomogeneous = false;

        ++lineLength;
        neighbor = *c;
      }

      assert(lineLength <= 80);
    }

    if(isHomogeneous)
    {
      puts("1");
      continue;
    }

    else if(isPrime[lineLength])
    {
      printf("%zu\n", lineLength);
      continue;
    }

    size_t index = 2;
    bool foundMatch = false;

    // Check blocks of increasing size until we find the first
    // repetition period or exhaust the string.
    for(;index <= (lineLength / 2); ++index)
    {
      for(size_t j = index; j < lineLength; j += index)
      {
        if(memcmp(lineBuffer, lineBuffer + j, index))
        {
          foundMatch = false;
          break;
        }

        foundMatch = true;
      }

      if(foundMatch) break;
    }

    printf("%zu\n", (foundMatch ? index : lineLength));
  }

  // The CRT takes care of cleanup.
}
