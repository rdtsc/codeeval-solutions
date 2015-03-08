#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

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

  static char stdoutBuffer[256] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[16] = "", * line = lineBuffer + 1;
      fgets(line, sizeof lineBuffer - 1, inputStream);
      line = lineBuffer + 1)
  {
    bool isSorted = true;
    size_t lineLength = 0;

    // Check if the input is sorted (descending), remove any trailing newlines,
    // and keep track of the string's length.
    for(char* c = line, *p = c + 1; *c; ++c, ++p)
    {
      ++lineLength;

      if(*p == '\n')
      {
        *p = '\0';
        break;
      }

      if(*p > *c) isSorted = false;
    }

    // No permutations left. Inject a zero at the front.
    if(isSorted)
    {
      *lineBuffer = '0';
      line = lineBuffer;
      ++lineLength;
    }

    nextPermutation(line, line + lineLength);
    puts(line);
  }

  // The CRT takes care of cleanup.
}
