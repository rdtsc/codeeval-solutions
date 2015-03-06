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
    if(*c == '\n' || *c == ',')
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

  for(char lineBuffer[768] = "", * line = lineBuffer;
      fgets(lineBuffer, sizeof lineBuffer, inputStream);
      line = lineBuffer)
  {
    int numbers[256] = {0};
    size_t numberCount = 0;

    // Extract the list of numbers.
    for(;*line; numbers[numberCount++] = stoi(&line))
    {
      assert(numberCount < sizeof numbers / sizeof *numbers);
    }

    int maxSum   = numbers[0],
        localMax = numbers[0];

    for(size_t i = 1; i < numberCount; ++i)
    {
      #define MAX(x, y) ((x) > (y) ? (x) : (y))

      localMax = MAX(numbers[i], localMax + numbers[i]);
      maxSum   = MAX(maxSum, localMax);

      #undef MAX
    }

    printf("%d\n", maxSum);
  }

  // The CRT takes care of cleanup.
}
