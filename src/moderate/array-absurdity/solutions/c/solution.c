#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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

  for(char lineBuffer[1024] = "", * line = lineBuffer;
      fgets(line, sizeof lineBuffer, inputStream);
      line = lineBuffer)
  {
    if(*line == '\n') continue;

    // Fetch the number of elements in the sequence.
    const unsigned n = atoi(line);

    while(*line && *line++ != ';');

    unsigned talliedArraySum = 0;

    // Sum the comma-separated terms in the array.
    while(*line)
    {
      talliedArraySum += atoi(line);

      while(*line && *line++ != ',');
    }

    // Expected sum for the arithmetic sequence: [0, N-2]
    const unsigned expectedArraySum = (n - 1) * (n - 2) / 2;

    // The difference between the manually tallied sum and the expected sum
    // exposes the duplicate term that's hidden away in the input array.
    printf("%u\n", talliedArraySum - expectedArraySum);
  }

  // The CRT takes care of cleanup.
}
