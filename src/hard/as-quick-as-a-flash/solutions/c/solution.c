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

static ptrdiff_t partition(unsigned* const values,
                           ptrdiff_t head,
                           ptrdiff_t tail)
{
  assert(values);

  // Hoare partition scheme.
  for(const unsigned pivot = values[head];;)
  {
    while(values[head] < pivot) ++head;
    while(values[tail] > pivot) --tail;

    if(head >= tail) return tail;

    const unsigned oldHeadValue = values[head];

    values[head] = values[tail];
    values[tail] = oldHeadValue;
  }

  assert(false);

  return 0;
}

static size_t doQuickSort(unsigned* const values,
                          const ptrdiff_t head,
                          const ptrdiff_t tail,
                          size_t partitionCount)
{
  assert(values);

  if(head >= tail) return partitionCount;

  const ptrdiff_t pivot = partition(values, head, tail);

  return doQuickSort(values, head, (pivot - 1), ++partitionCount) +
         doQuickSort(values, (pivot + 1), tail, 0);
}

static size_t quickSort(unsigned* const values, const size_t count)
{
  assert(values && count);

  return doQuickSort(values, 0, (count - 1), 0);
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

  for(char lineBuffer[256] = "";
    fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    unsigned values[32] = {0};
    size_t valueCount = 0;

    for(const char* cursor = lineBuffer; *cursor;)
    {
      assert(valueCount < (sizeof values / sizeof *values));

      values[valueCount++] = extractUint(&cursor);
    }

    printf("%zu\n", quickSort(values, valueCount));
  }

  // The CRT takes care of cleanup.
}
