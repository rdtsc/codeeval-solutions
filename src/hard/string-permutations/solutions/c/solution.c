#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline int asciiOrdering(const void* const lhs, const void* const rhs)
{
  assert(lhs && rhs);

  register const char x = *(const char*)lhs,
                      y = *(const char*)rhs;

  return (x < y) ? (-1) : (x > y);
}

// Generates the next lexicographical permutation (in-place).
static inline bool nextPermutation(char* const begin, char* const end)
{
  assert(begin && end && end > begin);

  const size_t rangeSize = end - begin - 1;
  size_t i = rangeSize, j = i;

  // Find the largest index I such that array[I] < array[I + 1]...
  while(i && begin[i - 1] >= begin[i]) --i;

  // ...if no such index exists, the permutation is the last permutation.
  if(!i) return false;

  // Find the largest index J greater than I such that array[I] < array[J].
  for(--i; begin[j] <= begin[i]; --j);

  #define SWAP(lhs, rhs) do { \
    const char oldLhs = (lhs); (lhs) = (rhs); (rhs) = oldLhs; \
  } while(false)

  // Swap the value of array[I] with [J].
  SWAP(begin[i], begin[j]);

  // Reverse the sequence from array[I + 1] up to the last element, inclusive.
  for(++i, j = rangeSize; j > i; ++i, --j) SWAP(begin[i], begin[j]);

  #undef SWAP

  return true;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[1536] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[16] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    size_t lineLength = strlen(lineBuffer);

    // Trim trailing newline, if applicable.
    if(lineLength && lineBuffer[lineLength - 1] == '\n')
    {
      lineBuffer[--lineLength] = '\0';
    }

    qsort(lineBuffer, lineLength, 1, asciiOrdering);
    fputs(lineBuffer, stdout);

    while(nextPermutation(lineBuffer, lineBuffer + lineLength))
    {
      putchar(',');
      fputs(lineBuffer, stdout);
    }

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
