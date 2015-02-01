#include <assert.h>
#include <stddef.h>
#include <stdio.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[768] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[768] = "", * line = lineBuffer;
      fgets(line, sizeof lineBuffer, inputStream);)
  {
    // The problem statement guarantees that the message will consist of at
    // least two words and that all characters are padded with the same offset.
    // Given the above, we can simply look for a character with the smallest
    // value in the encrypted message and be guaranteed that it's a space (0x20)
    // since that's the lowest printable character in the ASCII table. We can
    // therefore safely ignore the hint information provided at the front of the
    // input line by jumping straight to the encrypted payload.
    for(unsigned pipeCount = 0; *line; ++line)
    {
      if(*line == '|' && ++pipeCount == 2) break;
    }

    struct
    {
      size_t count;
      unsigned char data[sizeof lineBuffer / 2];
    } values = {0};

    unsigned char encryptionOffset = 0xFF;

    // Read the encrypted data into `values` and find the lowest-valued
    // character which is equal to: 0x20 + offset
    {
      unsigned n = 0;
      int bytesRead = 0;

      for(char* chunk = line + (*line == '|');
          sscanf(chunk, "%u%n", &n, &bytesRead) == 1;
          chunk += bytesRead)
      {
        if(n < encryptionOffset) encryptionOffset = n;

        if(values.count > sizeof values.data) break;

        values.data[values.count++] = n;
      }

      // Reconstruct the encryption offset.
      encryptionOffset -= ' ';
    }

    // Decrypt the characters.
    for(size_t i = 0; i < values.count; ++i)
    {
      values.data[i] -= encryptionOffset;
    }

    puts((const char*)values.data);
  }

  // The CRT takes care of cleanup.
}
