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

  static char stdoutBuffer[256] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    unsigned constraints[3] = {0};

    const char* cursor = lineBuffer;

    // Yields a small gain in performance over sscanf and friends in CE's
    // environment due to naive parsing rules and lack of locale awareness on
    // our part.
    {
      unsigned term = 0;
      size_t i = 0;

      static const size_t constraintCount =
        (sizeof constraints / sizeof *constraints);

      for(const char* c = cursor;; ++c)
      {
        if(*c >= '0' && *c <= '9') term = (term * 10) + (*c - '0');

        else if(*c == ' ' || *c == '\n' || !*c)
        {
          constraints[i++] = term;
          term = 0;

          if(*c == '\n' || !*c || i == constraintCount)
          {
            cursor = c;
            break;
          }
        }
      }
    }

    static const unsigned wallTolerance = 6;

    const unsigned wireLength = constraints[0],
                   batTolerance = constraints[1],
                   existingBatCount = constraints[2];

    unsigned roomLeft = 0;

    // Clean wire.
    if(!existingBatCount)
    {
      // Bats don't sit closer than 6 units to a building.
      assert(wireLength >= (wallTolerance * 2));

      roomLeft = ((wireLength - (wallTolerance * 2)) / batTolerance) + 1;
    }

    // Some bats are already on the wire.
    else
    {
      // Sanity check. Unrestricted VLAs can lead to UB if allocation fails.
      assert(existingBatCount <= 1e3);

      // VLAs yield a slightly better memory score on CodeEval than straight
      // [m,c]alloc. Shouldn't rely on this in C11 and onwards, though.
      unsigned batPositions[existingBatCount];
      memset(batPositions, 0, sizeof batPositions);

      // Fetch existing positions of bats on the wire. Ditto regarding speed
      // in CE's environment.
      {
        unsigned term = 0;
        size_t i = 0;

        while(*cursor == ' ') ++cursor;

        for(const char* c = cursor;; ++c)
        {
          if(*c >= '0' && *c <= '9') term = (term * 10) + (*c - '0');

          else if(*c == ' ' || *c == '\n' || !*c)
          {
            batPositions[i++] = term;
            term = 0;

            if(*c == '\n' || !*c || i == existingBatCount) break;
          }
        }
      }

      const unsigned upperLimit = (wireLength - wallTolerance);

      // Find open room on the wire.
      for(unsigned i = wallTolerance; i <= upperLimit; i += batTolerance)
      {
        bool foundRoom = true;

        for(unsigned j = 0; j < existingBatCount; ++j)
        {
          const unsigned pos  = batPositions[j],
                         dist = ((pos > i) ? (pos - i) : (i - pos));

          // Are we within spec?
          if(dist < batTolerance)
          {
            foundRoom = false;
            i = pos;
            break;
          }
        }

        if(foundRoom) ++roomLeft;
      }
    }

    printf("%u\n", roomLeft);
  }

  // The CRT takes care of cleanup.
}
