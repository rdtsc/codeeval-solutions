#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

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

  for(unsigned doorCount = 0, iterations = 0;
      fscanf(inputStream, "%u %u", &doorCount, &iterations) == 2;)
  {
    // Sanity check. Unrestricted VLAs can lead to UB if allocation fails.
    assert(doorCount <= 8192);

    // VLAs yield a slightly better memory score on CodeEval than straight
    // [m,c]alloc. Shouldn't rely on this in C11 and onwards, though.
    bool locks[doorCount];
    memset(locks, false, sizeof locks);

    #define EVERY(Index, Counter) for(unsigned (Counter) = (Index) - 1; \
      (Counter) < doorCount; (Counter) += (Index))

    #define LOCK(Index) locks[(Index)] = true
    #define FLIP(Index) locks[(Index)] = !locks[(Index)]

    for(unsigned i = 1; i < iterations; ++i)
    {
      EVERY(2, j) LOCK(j);
      EVERY(3, j) FLIP(j);
    }

    // Last iteration consists of a single op.
    FLIP(doorCount - 1);

    #undef FLIP
    #undef LOCK
    #undef EVERY

    unsigned unlockedDoorCount = 0;

    for(unsigned i = 0; i < doorCount; ++i) if(!locks[i]) ++unlockedDoorCount;

    printf("%u\n", unlockedDoorCount);
  }

  // The CRT takes care of cleanup.
}
