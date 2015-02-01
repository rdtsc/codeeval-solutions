#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
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

  for(char lineBuffer[256] = "", * line = lineBuffer;
      fgets(line, sizeof lineBuffer, inputStream);
      line = lineBuffer)
  {
    typedef unsigned char byte;

    byte lcdMasks[12] = {0};
    int bytesRead = 0;

    // Fetch the display damage masks.
    for(size_t i = 0; i < sizeof lcdMasks; ++i, line += bytesRead)
    {
      char mask[8] = "";

      sscanf(line, "%8s%n", mask, &bytesRead);

      for(size_t j = 0; j < sizeof mask; ++j)
      {
        lcdMasks[i] += (mask[j] & 1) * (1 << (sizeof(mask) - j - 1));
      }
    }

    char number[32] = "";

    sscanf(++line, "%13s%n", number, &bytesRead);

    byte digitMasks[sizeof lcdMasks] = {0};
    size_t maskIndex = 0;

    // Convert the digits of the number to be shown into segment toggle masks.
    for(size_t i = 0; i < (size_t)bytesRead; ++i)
    {
      static const byte baseDigitMasks[] =
      {
        /*
          7-segment digit mappings with the 8th segment (dot) turned off.

          0: 11111100   1: 01100000   2: 11011010   3: 11110010   4: 01100110
          5: 10110110   6: 10111110   7: 11100000   8: 11111110   9: 11110110
        */

        0xFC, 0x60, 0xDA, 0xF2, 0x66,
        0xB6, 0xBE, 0xE0, 0xFE, 0xF6
      };

      if(isdigit(number[i]))
      {
        digitMasks[maskIndex++] = baseDigitMasks[number[i] - '0'];
      }

      // The decimal belongs to the previous number's 8th segment.
      else if(number[i] == '.')
      {
        digitMasks[maskIndex - 1] = digitMasks[maskIndex - 1] + 1;
      }
    }

    bool canDisplay = true;

    // Attempt to place the digits on the display from left to right.
    for(size_t i = 0; i < sizeof(lcdMasks) - maskIndex + 1; ++i)
    {
      canDisplay = true;

      for(size_t j = 0; j < maskIndex; ++j)
      {
        if((lcdMasks[i + j] & digitMasks[j]) != digitMasks[j])
        {
          canDisplay = false;
          break;
        }
      }

      if(canDisplay) break;
    }

    puts(canDisplay ? "1" : "0");
  }

  // The CRT takes care of cleanup.
}
