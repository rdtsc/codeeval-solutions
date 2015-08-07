#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// Yields a small gain in performance over sscanf and friends in CE's
// environment due to naive parsing rules and lack of locale awareness
// on our part.
static int extractInt(const char** string)
{
  assert(string && *string);

  int result = 0;
  bool isNegative = false;
  const char* c = *string;

  #define IS_PAYLOAD() ((*c >= '0' && *c <= '9') || *c == '-')

  // Eat anything that's not a digit.
  while(*c && !IS_PAYLOAD()) ++c;

  for(;*c; ++c)
  {
    if(!IS_PAYLOAD())
    {
      ++c;
      break;
    }

    else if(*c == '-') isNegative = true;

    else result = (result * 10) + (*c - '0');
  }

  #undef IS_PAYLOAD

  *string = c;

  return (isNegative ? -result : result);
}

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

  for(char lineBuffer[1536] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    static int values[32] = {0};

    static const size_t maxColumnCount = (sizeof values / sizeof *values);

    for(size_t i = 0; i < maxColumnCount; ++i) values[i] = INT_MIN;

    for(const char* chunk = strtok(lineBuffer, "|");
        chunk; chunk = strtok(NULL, "|"))
    {
      // Retain the highest values in each column.
      for(size_t columnIndex = 0; *chunk; ++columnIndex)
      {
        assert(columnIndex < (sizeof values / sizeof *values));

        const int value = extractInt(&chunk);

        if(value > values[columnIndex]) values[columnIndex] = value;
      }
    }

    for(size_t i = 0; i < maxColumnCount; ++i)
    {
      if(values[i] == INT_MIN) break;

      printf("%d ", values[i]);
    }

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
