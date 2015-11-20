#include <assert.h>
#include <stddef.h>
#include <stdio.h>

static unsigned getCombinationCount(const int value,
                                    const size_t index,
                                    const unsigned denominations[index])
{
  assert(denominations);

  if(value <= 0 || !index) return !value;

  #define SELF(Value, Index) \
    getCombinationCount((Value), (Index), denominations)

  const unsigned lhs = SELF(value, (index - 1)),
                 rhs = SELF((value - denominations[index - 1]), index);

  return (lhs + rhs);

  #undef SELF
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

  static const unsigned denominations[] = {1, 5, 10, 25, 50};

  static const size_t denominationCount =
    (sizeof denominations / sizeof *denominations);

  static unsigned memo[100] = {0};

  for(unsigned n = 0; fscanf(inputStream, "%u", &n) == 1;)
  {
    // As per the problem statement.
    assert((n >= 1) && (n <= (sizeof memo / sizeof *memo)));

    if(!memo[n - 1])
    {
      memo[n - 1] = getCombinationCount(n, denominationCount, denominations);
    }

    printf("%u\n", memo[n - 1]);
  }

  // The CRT takes care of cleanup.
}
