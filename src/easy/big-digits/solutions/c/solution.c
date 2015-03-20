#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[5120] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  // Slightly faster to split the digits beforehand than printf-ing: "%.5s".
  static const char* const art[] =
  {
    /*
      Transformations: /(.{5})/ -> "$1\\0"
                       /^|$/    -> "\""
                       /$/      -> ","
    */

    "-**--\0--*--\0***--\0***--\0-*---\0****-\0-**--\0****-\0-**--\0-**--",
    "*--*-\0-**--\0---*-\0---*-\0*--*-\0*----\0*----\0---*-\0*--*-\0*--*-",
    "*--*-\0--*--\0-**--\0-**--\0****-\0***--\0***--\0--*--\0-**--\0-***-",
    "*--*-\0--*--\0*----\0---*-\0---*-\0---*-\0*--*-\0-*---\0*--*-\0---*-",
    "-**--\0-***-\0****-\0***--\0---*-\0***--\0-**--\0-*---\0-**--\0-**--",
    "-----\0-----\0-----\0-----\0-----\0-----\0-----\0-----\0-----\0-----"
  };

  static const size_t digitHeight = sizeof art / sizeof *art,
                      digitWidth  = 6;

  for(char lineBuffer[32] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    char digits[16] = {0};
    size_t digitCount = 0;

    // Only interested in digits.
    for(const char* c = lineBuffer; *c && *c != '\n'; ++c) if(isdigit(*c))
    {
      digits[digitCount++] = *c - '0';
    }

    if(!digitCount) continue;

    // Jump to the start of the current digit in scanline Y and print the
    // next 5 characters.
    for(size_t y = 0; y < digitHeight; ++y, putchar('\n'))
    for(size_t i = 0; i < digitCount;  ++i)
    {
      fputs(art[y] + digits[i] * digitWidth, stdout);
    }
  }

  // The CRT takes care of cleanup.
}
