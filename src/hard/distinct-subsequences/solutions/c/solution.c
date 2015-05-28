#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static inline size_t getDistinctSubsequenceCount(const char* const sequence,
                                                 const char* const subsequence)
{
  assert(sequence && subsequence);

  // Probably should ask the caller for these, as this work was already
  // done during parsing.
  const size_t seqLength = strlen(sequence),
               subLength = strlen(subsequence);

  // Sanity check. Unrestricted VLAs can lead to UB if allocation fails.
  assert(subLength <= 512);

  // VLAs yield a slightly better memory score on CodeEval than straight
  // [m,c]alloc. Shouldn't rely on this in C11 and onwards, though.
  size_t count[subLength + 1];
  memset(count, 0, sizeof count);

  // Seed.
  count[subLength] = 1;

  // Fill the result table, RTL.
  for(size_t i = seqLength; i--;)
  for(size_t j = 0; j < subLength; ++j)
  {
    // Tally match.
    if(sequence[i] == subsequence[j])
    {
      count[j] += count[j + 1];
    }
  }

  return *count;
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

  for(char lineBuffer[1024] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    const char* seq = lineBuffer,
              * sub = lineBuffer;

    // Isolate the sequence and subsequence chunks.
    {
      char* delimiter = strchr(lineBuffer, ',');

      assert(delimiter && *delimiter == ',');

      // Kill the comma.
      *delimiter++ = '\0';

      sub = delimiter;

      // Trim the subsequence.
      for(char* c = delimiter; *c; ++c) if(*c == '\n') *c = '\0';
    }

    assert(sub > seq);

    printf("%zu\n", getDistinctSubsequenceCount(seq, sub));
  }

  // The CRT takes care of cleanup.
}
