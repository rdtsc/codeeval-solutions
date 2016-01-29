#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

// Yields a small gain in performance over sscanf and friends in CE's
// environment due to naive parsing rules and lack of locale awareness
// on our part.
static unsigned extractUint(const char** string)
{
  assert(string && *string);

  unsigned result = 0;
  const char* c = *string;

  #define IS_PAYLOAD() (*c >= '0' && *c <= '9')

  // Eat anything that's not a digit.
  while(*c && !IS_PAYLOAD()) ++c;

  for(;*c; ++c)
  {
    if(!IS_PAYLOAD())
    {
      ++c;
      break;
    }

    else result = (result * 10) + (*c - '0');
  }

  #undef IS_PAYLOAD

  *string = c;

  return result;
}

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

  static const float shrinkFactor = 1.25;

  for(char lineBuffer[256] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    unsigned values[64] = {0};
    size_t valueCount = 0;

    for(const char* cursor = lineBuffer; *cursor;)
    {
      assert(valueCount < (sizeof values / sizeof *values));

      values[valueCount++] = extractUint(&cursor);
    }

    size_t result = 0;

    for(size_t margin = valueCount;; ++result)
    {
      if(margin > 1) margin /= shrinkFactor;

      bool didWork = false;

      for(size_t i = 0, j = margin; j < valueCount; ++i, ++j)
      {
        if(values[i] > values[j])
        {
          didWork = true;

          values[i] ^= values[j];
          values[j] ^= values[i];
          values[i] ^= values[j];
        }
      }

      if(!didWork && (margin == 1)) break;
    }

    printf("%zu\n", result);
  }

  // The CRT takes care of cleanup.
}
