#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

static unsigned doEggStrengthTest(const unsigned eggCount,
                                  const unsigned floorCount)
{
  if(eggCount == 1 || floorCount == 1) return floorCount;

  else if(!eggCount || !floorCount) return 0;

  // As described in:
  // https://en.wikipedia.org/wiki/Dynamic_programming#Egg_dropping_puzzle

  const unsigned broken = doEggStrengthTest(eggCount - 1, floorCount - 1),
                 intact = doEggStrengthTest(eggCount - 0, floorCount - 1);

  return (broken + intact + 1);
}

static unsigned getTrialCount(const unsigned eggCount,
                              const unsigned floorCount)
{
  assert(eggCount && floorCount);

  for(unsigned result = 0; result < floorCount; ++result)
  {
    if(doEggStrengthTest(eggCount, result) >= floorCount) return result;
  }

  return 0;
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

  for(unsigned eggCount = 0, floorCount = 0;
      fscanf(inputStream, "%u %u", &eggCount, &floorCount) == 2;)
  {
    printf("%u\n", getTrialCount(eggCount, floorCount));
  }

  // The CRT takes care of cleanup.
}
