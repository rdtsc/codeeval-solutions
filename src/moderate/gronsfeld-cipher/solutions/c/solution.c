#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[8192] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  // The supplied alphabet is *almost* a contiguous block of ASCII characters.
  static const char alphabet[] = " !\"#$%&'()*+,-./0123456789:<=>?@ABCDEFGHIJ"
                                 "KLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    char* key = strchr(lineBuffer, ';'), * message = key;

    assert(key);

    // Isolate the key/encrypted message chunks.
    *message++ = '\0';
    key = lineBuffer;

    char* c = message;

    for(;*c && *c != '\n'; ++c)
    {
      const size_t keyOffset = (*key - '0');

      size_t alphaOffset = strchr(alphabet, *c) - alphabet;

      // Find the correct character in the alphabet.
      alphaOffset = (alphaOffset >= keyOffset) ?
                    (alphaOffset  - keyOffset) :
                    (sizeof(alphabet) - (keyOffset - alphaOffset) - 1);

      *c = alphabet[alphaOffset];

      // The key is circular.
      if(!*(++key)) key = lineBuffer;
    }

    // Remove any trailing newlines from the message.
    *c = '\0';

    puts(message);
  }

  // The CRT takes care of cleanup.
}
