#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
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

  for(char lineBuffer[128] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    const char* players[16] = {NULL};
    size_t playerCount = 0;
    unsigned target = 0;

    // Fetch (zero-based) black card index.
    {
      char* cursor = strchr(lineBuffer, '|');

      assert(cursor);

      // Kill the pipe.
      *cursor++ = '\0';

      target = strtol(cursor, NULL, 10);

      assert((target >= 3) && (target <= 15));

      --target;
    }

    // Fetch player names.
    for(const char* cursor = strtok(lineBuffer, " ");
        cursor; cursor = strtok(NULL, " "))
    {
      assert(playerCount < (sizeof players / sizeof *players));

      players[playerCount++] = cursor;
    }

    // Eliminate players.
    for(;playerCount > 1; --playerCount)
    {
      const size_t mark = (target % playerCount),
                   rift = (playerCount - mark);

      memmove(&players[mark], &players[mark + 1], rift * sizeof(*players));
    }

    // Last player standing.
    puts(*players);
  }

  // The CRT takes care of cleanup.
}
