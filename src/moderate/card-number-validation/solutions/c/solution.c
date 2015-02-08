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

  static char stdoutBuffer[256] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[32] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    // Sum of the card digits after applying the Luhn algorithm.
    unsigned digitChecksum = 0;
    size_t digitIndex = 0;

    // Read card digits RTL.
    for(const char* c = lineBuffer + strlen(lineBuffer) - 1; *c; --c)
    {
      if(!isdigit(*c)) continue;

      // Odd digits are added to the checksum verbatim.
      else if(++digitIndex & 1) digitChecksum += *c - '0';

      else
      {
        // Even digits are doubled...
        const unsigned char digit = (*c - '0') * 2;

        // ...and if the result is >= 10, the resulting digits are
        // summed together and added to the checksum.
        digitChecksum += (digit >= 10) ? (digit - 9) : (digit);
      }
    }

    // Card number is considered valid if its checksum is a multiple of 10.
    puts((digitChecksum % 10) ? "0" : "1");
  }

  // The CRT takes care of cleanup.
}
