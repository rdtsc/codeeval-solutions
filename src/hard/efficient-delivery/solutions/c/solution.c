#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

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

static bool canDeliver(const int targetVolume,
                       const size_t index,
                       const unsigned capacities[index])
{
  assert(capacities);

  if(targetVolume <= 0 || !index) return !targetVolume;

  #define SELF(Value, Index) \
    canDeliver((Value), (Index), capacities)

  const bool lhs = SELF(targetVolume, (index - 1)),
             rhs = SELF((targetVolume - capacities[index - 1]), index);

  return (lhs || rhs);

  #undef SELF
}

static void doDumpDeliveryOptions(const size_t index,
                                  const unsigned targetVolume,
                                  const unsigned* const capacities,
                                  const size_t capacityCount,
                                  size_t* const tankers,
                                  FILE* const outputStream)
{
  assert(index <= capacityCount);

  // Last option, evenly divisible.
  if(index == (capacityCount - 1))
  {
    if(!(targetVolume % capacities[index]))
    {
      tankers[index] = (targetVolume / capacities[index]);

      doDumpDeliveryOptions((index + 1), 0, capacities,
        capacityCount, tankers, outputStream);
    }
  }

  // Done.
  else if(index == capacityCount)
  {
    fprintf(outputStream, "[%zu", *tankers);

    for(size_t i = 1; i < capacityCount; ++i)
    {
      fprintf(outputStream, ",%zu", tankers[i]);
    }

    fputc(']', outputStream);
  }

  // Try next arrangements.
  else for(size_t i = 0; i <= (targetVolume / capacities[index]); ++i)
  {
    tankers[index] = i;

    doDumpDeliveryOptions((index + 1), (targetVolume - (i * capacities[index])),
      capacities, capacityCount, tankers, outputStream);
  }
}

static void dumpDeliveryOptions(const unsigned* const capacities,
                                const size_t capacityCount,
                                const unsigned targetVolume,
                                FILE* const outputStream)
{
  // Tanker count.
  assert(capacities && (capacityCount >= 2) && (capacityCount <= 5));

  // Target oil volume.
  assert((targetVolume >= 1) && (targetVolume <= 2e5));

  assert(outputStream && !ferror(outputStream));

  // VLAs yield a slightly better memory score on CodeEval than straight
  // [m,c]alloc. Shouldn't rely on this in C11 and onwards, though.
  size_t tankers[capacityCount];
  memset(tankers, 0, sizeof tankers);

  #define CAN_DELIVER(Volume) \
    canDeliver((Volume), capacityCount, capacities)

  // Options exist as is.
  if(CAN_DELIVER(targetVolume))
  {
    doDumpDeliveryOptions(0, targetVolume, capacities,
      capacityCount, tankers, outputStream);

    fputc('\n', outputStream);
  }

  // Inflate target oil volume until delivery options become possible.
  else for(unsigned volume = targetVolume;;) if(CAN_DELIVER(++volume))
  {
    fprintf(outputStream, "%u\n", (volume - targetVolume));

    break;
  }

  #undef CAN_DELIVER
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[16384] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[256] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    unsigned values[8] = {0};
    size_t valueCount = 0;

    for(const char* cursor = lineBuffer; *cursor;)
    {
      assert(valueCount < (sizeof values / sizeof *values));

      values[valueCount++] = extractUint(&cursor);
    }

    assert(valueCount);

    const unsigned targetVolume = values[--valueCount];

    dumpDeliveryOptions(values, valueCount, targetVolume, stdout);
  }

  // The CRT takes care of cleanup.
}
