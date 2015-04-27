#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

static inline bool isPalindrome(const unsigned n)
{
  unsigned reversed = 0;

  for(unsigned i = n; i; i /= 10)
  {
    reversed = (reversed * 10) + (i % 10);
  }

  return (reversed == n);
}

static inline bool isInteresting(const unsigned lowerBound,
                                 const unsigned upperBound)
{
  static bool memo[1000] = {false}, cache[1000] = {false};
  static const size_t memoThreshold = (sizeof memo / sizeof *memo);

  assert(memoThreshold == (sizeof cache / sizeof *cache));

  size_t palindromeCount = 0;

  // Count the number of palindromes in current range.
  for(unsigned i = lowerBound; i <= upperBound; ++i)
  {
    if(i < memoThreshold)
    {
      if(!memo[i])
      {
        memo[i] = true;
        cache[i] = isPalindrome(i);
      }

      palindromeCount += cache[i];
    }

    else palindromeCount += isPalindrome(i);
  }

  // A range is considered to be "interesting" if it contains
  // an even number of palindromic terms.
  return !(palindromeCount & 1);
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[128] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(unsigned lowerBound = 0, upperBound = 0;
      fscanf(inputStream, "%u %u", &lowerBound, &upperBound) == 2;)
  {
    size_t interestingRangeCount = 0;

    for(unsigned i = lowerBound; i <= upperBound; ++i)
    {
      for(unsigned j = i; j <= upperBound; ++j)
      {
        interestingRangeCount += isInteresting(i, j);
      }
    }

    printf("%zu\n", interestingRangeCount);
  }

  // The CRT takes care of cleanup.
}
