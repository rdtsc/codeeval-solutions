#include <assert.h>
#include <inttypes.h>
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

  static char stdoutBuffer[256] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  typedef uint_fast64_t value_type;

  for(char lineBuffer[1536] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    const char* cursor = lineBuffer;

    value_type coinLimit  = extractUint(&cursor),
               valueLimit = extractUint(&cursor);

    assert(coinLimit  && (coinLimit  <= 100));
    assert(valueLimit && (valueLimit <= 1e9));

    static value_type denominations[100] = {0};

    static const size_t denominationCapacity =
      (sizeof denominations / sizeof *denominations);

    size_t denominationCount = 0;

    while(*cursor)
    {
      assert(denominationCount < denominationCapacity);
      denominations[denominationCount++] = extractUint(&cursor);
    }

    assert(denominationCount && (denominationCount <= 100));

    value_type result = 0,
               target = 1;

    for(size_t i = 0; i < denominationCount; ++i)
    {
      for(;denominations[i] > target; ++result)
      {
        target = (coinLimit + 1) * target;
      }

      target += (denominations[i] * coinLimit);
    }

    for(;target < valueLimit; ++result)
    {
      target = (coinLimit + 1) * target;
    }

    printf("%" PRIuFAST64 "\n", result);
  }

  // The CRT takes care of cleanup.
}
