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

  static char stdoutBuffer[512] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  bool isFirstSlice = true;
  size_t lastPortalIndex = 0;

  for(char lineBuffer[16] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);
      isFirstSlice = false)
  {
    // Each track slice is at most 12 characters wide (not including any CR/LF).
    assert(strlen(lineBuffer) <= 13);

    // Each track slice has at most 2 portals.
    struct
    {
      size_t index;
      bool exists;
    } portals[2] = {{0}};

    enum {CHECKPOINT, GATE};

    // Fetch portal positions in each track slice.
    for(size_t i = 0; i < 12; ++i)
    {
      switch(lineBuffer[i]) case 'C': case '_':
      {
        const size_t type = (lineBuffer[i] == 'C' ? CHECKPOINT : GATE);

        portals[type].index = i;
        portals[type].exists = true;

        if(portals[CHECKPOINT].exists && portals[GATE].exists) break;
      }
    }

    // Patch the current track slice with the appropriate turn symbol based on
    // the position of the previous portal. Checkpoints are given priority by
    // being checked for first.
    for(size_t i = 0; i < 2; ++i) if(portals[i].exists)
    {
      const size_t index = portals[i].index;

      #define IS_STRAIGHT (isFirstSlice || lastPortalIndex == index)
      #define IS_LEFT (lastPortalIndex > index)

      const char direction = (IS_STRAIGHT ? '|' : (IS_LEFT ? '/' : '\\'));

      #undef IS_LEFT
      #undef IS_STRAIGHT

      lineBuffer[index] = direction;
      lastPortalIndex = index;

      break;
    }

    fputs(lineBuffer, stdout);
  }

  // The CRT takes care of cleanup.
}
