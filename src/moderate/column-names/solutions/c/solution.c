#include <assert.h>
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

  // Output is guaranteed to be no more than 3 characters long.
  char outputBuffer[16] = "";

  for(unsigned n = 0; fscanf(inputStream, "%u", &n) == 1;)
  {
    char* out = (outputBuffer + sizeof outputBuffer - 2);

    // Emit characters backwards.
    for(;n--; n /= 26)
    {
      assert(out >= outputBuffer);
      *out-- = ('A' + (n % 26));
    }

    puts(++out);
  }

  // The CRT takes care of cleanup.
}
