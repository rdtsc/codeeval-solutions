#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>

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

  for(char lineBuffer[64] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    assert(isdigit(*lineBuffer));

    struct
    {
      unsigned char id, value;
    } players[32] = {{0}};

    size_t playerCount = 0;
    unsigned char values[10] = {0};

    // Record player stats.
    for(const char* c = lineBuffer; *c && *c != '\n'; ++c) if(isdigit(*c))
    {
      const unsigned char digit = *c - '0';

      ++values[digit];
      players[playerCount].id = playerCount + 1;
      players[playerCount++].value = digit;
    }

    unsigned char winner = 0;

    // Find the lowest unique ballot.
    for(size_t i = 1; i < sizeof values; ++i) if(values[i] == 1)
    {
      winner = i;
      break;
    }

    if(!winner) puts("0");

    // Find the player with the correct ballot.
    else for(size_t i = 0; i < playerCount; ++i)
    {
      if(players[i].value == winner)
      {
        printf("%u\n", players[i].id);
        break;
      }
    }
  }

  // The CRT takes care of cleanup.
}
