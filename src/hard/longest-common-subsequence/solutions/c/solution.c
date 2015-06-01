#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static int dumpLcs(const char* const lhs, const char* const rhs)
{
  assert(lhs && rhs);

  const size_t lhsSize = strlen(lhs),
               rhsSize = strlen(rhs);

  assert(lhsSize && rhsSize);

  // Sanity check. Unrestricted VLAs can lead to UB if allocation fails.
  assert(lhsSize <= 512 && rhsSize <= 512);

  // VLAs yield a slightly better memory score on CodeEval than straight
  // [m,c]alloc. Shouldn't rely on this in C11 and onwards, though.
  unsigned table[lhsSize + 1][rhsSize + 1];
  memset(table, 0, sizeof table);

  // Per: https://en.wikipedia.org/wiki/Longest_common_subsequence_problem
  for(const char* a = lhs; *a; ++a)
  for(const char* b = rhs; *b; ++b)
  {
    #define VAL(xOffset, yOffset) \
      table[(a - lhs) + (yOffset)][(b - rhs) + (xOffset)]

    if(*a != *b) VAL(1, 1) = VAL(1, 0) > VAL(0, 1) ?
                             VAL(1, 0) : VAL(0, 1);

    else VAL(1, 1) = VAL(0, 0) + 1;

    #undef VAL
  }

  char outputBuffer[512] = "", * out = &outputBuffer[sizeof outputBuffer - 1];

  // Reconstruct the subsequence from the bottom-right to the top-left.
  for(int y = lhsSize, x = rhsSize; (y > 0 && x > 0); --y, --x)
  {
    const unsigned this = table[y][x],
                   top  = table[y - 1][x],
                   left = table[y][x - 1];

    if(this == top) ++x;

    else if(this == left) ++y;

    else *--out = lhs[y - 1];
  }

  return puts(out);
}

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

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    const char* lhs = lineBuffer,
              * rhs = lineBuffer;

    // Isolate the two strings.
    {
      char* delimiter = strchr(lineBuffer, ';');

      assert(delimiter && *delimiter == ';');

      // Kill the semicolon.
      *delimiter++ = '\0';

      rhs = delimiter;

      // Trim the right string.
      for(char* c = delimiter; *c; ++c) if(*c == '\n') *c = '\0';
    }

    dumpLcs(lhs, rhs);
  }

  // The CRT takes care of cleanup.
}
