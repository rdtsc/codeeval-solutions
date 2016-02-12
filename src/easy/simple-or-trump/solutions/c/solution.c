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

  static char stdoutBuffer[256] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  static const unsigned char mapping[] =
  {
    ['1'] = 10, ['2'] = 2, ['3'] = 3,
    ['4'] =  4, ['5'] = 5, ['6'] = 6,
    ['7'] =  7, ['8'] = 8, ['9'] = 9,

    ['J'] = 11, ['Q'] = 12,
    ['K'] = 13, ['A'] = 14,
  };

  for(char lhs[4] = "", rhs[4] = "", trumpSuit = '\0';
      fscanf(inputStream, "%3s %3s | %c", lhs, rhs, &trumpSuit) == 3;)
  {
    assert((strlen(lhs) >= 2) && (strlen(lhs) <= 3));
    assert((strlen(rhs) >= 2) && (strlen(rhs) <= 3));

    const unsigned lhsShift = ((lhs[strlen(lhs) - 1] == trumpSuit) * 3),
                   rhsShift = ((rhs[strlen(rhs) - 1] == trumpSuit) * 3);

    const unsigned a = (mapping[(size_t)*lhs] << lhsShift),
                   b = (mapping[(size_t)*rhs] << rhsShift);

    if(a == b) printf("%s %s\n", lhs, rhs);

    else puts(a > b ? lhs : rhs);
  }

  // The CRT takes care of cleanup.
}
