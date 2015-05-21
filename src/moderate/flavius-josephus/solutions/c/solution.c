#include <assert.h>
#include <stddef.h>
#include <stdio.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[4096] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(unsigned n = 0, m = 0; fscanf(inputStream, "%u,%u", &n, &m) == 2;)
  {
    // Sanity check. Unrestricted VLAs can lead to UB if allocation fails.
    assert(n && n <= 1e3);

    // VLAs yield a slightly better memory score on CodeEval than straight
    // [m,c]alloc. Shouldn't rely on this in C11 and onwards, though.
    unsigned roster[n];

    for(size_t i = 0; i < n; ++i) roster[i] = (i + 1);

    // 0-based count of people.
    for(size_t peopleLeft = n, i = (m - 1); peopleLeft;)
    {
      printf("%u ", roster[i] - 1);

      // RIP.
      roster[i] = 0;

      if(--peopleLeft) for(size_t j = 0; j < m;)
      {
        // Rewind to start of sequence.
        if(++i >= n) i = 0;

        // Is this a live person?
        if(roster[i]) ++j;
      }
    }

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
