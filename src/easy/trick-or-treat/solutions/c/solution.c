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

  static const char* const scanFormat =
    "%*[^:]:%u%*[^:]:%u%*[^:]:%u%*[^:]:%u";

  for(unsigned v = 0, z = 0, w = 0, h = 0;
      fscanf(inputStream, scanFormat, &v, &z, &w, &h) == 4;)
  {
    const unsigned take = v * 3 + // Vampires.
                          z * 4 + // Zombies.
                          w * 5;  // Witches.

    const unsigned loot   = (take * h),
                   shares = (v + z + w);

    printf("%u\n", loot / shares);
  }

  // The CRT takes care of cleanup.
}
