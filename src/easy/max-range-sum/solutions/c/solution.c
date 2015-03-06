#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

// Yields a small gain in performance over sscanf/atoi in CE's environment.
static inline int stoi(char** head)
{
  assert(head && *head);

  int result = 0;
  bool isNegative = false;

  char* c = *head;

  for(;*c; ++c)
  {
    if(*c == '\n' || *c == ' ')
    {
      ++c;
      break;
    }

    else if(*c == '-') isNegative = true;

    else result = (result * 10) + (*c - '0');
  }

  *head = c;

  return (isNegative) ? (-result) : (result);
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

  for(char lineBuffer[512] = "", * line = lineBuffer;
      fgets(lineBuffer, sizeof lineBuffer, inputStream);
      line = lineBuffer)
  {
    size_t period = 0;

    // Number of days we're going to be playing the market.
    while(*line && *line != ';')
    {
      period = (period * 10) + (*line++ - '0');
    }

    // No more than 100 days worth of data are provided.
    int deltas[100] = {0};
    size_t deltaCount = 0;

    // Daily portfolio gains/losses.
    for(++line; *line; deltas[deltaCount++] = stoi(&line))
    {
      assert(deltaCount < sizeof deltas / sizeof *deltas);
    }

    assert(period <= deltaCount);

    int maxGain = 0, localSum = 0;

    for(size_t i = 0; i < period; ++i) localSum += deltas[i];

    for(size_t i = period; i <= deltaCount; ++i)
    {
      if(localSum > maxGain) maxGain = localSum;

      localSum += deltas[i] - deltas[i - period];
    }

    printf("%d\n", maxGain);
  }

  // The CRT takes care of cleanup.
}
