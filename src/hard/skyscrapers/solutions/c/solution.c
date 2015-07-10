#include <assert.h>
#include <limits.h>
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

// A bit faster than printf in this use case.
static inline size_t appendTo(char* buffer, const unsigned n)
{
  assert(buffer);

  const char* const bufferBegin = buffer;

  #define THRESHOLD(Value) n < (1e##Value) ? (1e##Value) / 10

  const unsigned divisor = THRESHOLD(1) : THRESHOLD(2) :
                           THRESHOLD(3) : THRESHOLD(4) :
                           THRESHOLD(5) : 0;

  #undef THRESHOLD

  for(unsigned i = divisor; i; i /= 10)
  {
    *buffer++ = ('0' + (n / i) % 10);
  }

  return (buffer - bufferBegin);
}

static void dumpSkyline(const unsigned* const skyline,
                        const unsigned left,
                        const unsigned right,
                        FILE* const outputStream)
{
  assert(skyline && (right > left) && outputStream && !ferror(outputStream));

  static char outputBuffer[1536] = "";
  char* out = outputBuffer;

  #define EMIT(Char) (*out++ = (Char))

  for(unsigned trace = 0, i = left; i <= right; ++i)
  {
    const bool isVertex = (skyline[i] != trace);

    if(isVertex)
    {
      out += appendTo(out, i);
      EMIT(' ');

      out += appendTo(out, skyline[i]);
      EMIT(' ');

      trace = skyline[i];
    }
  }

  EMIT('\n');
  EMIT('\0');

  #undef EMIT

  fputs(outputBuffer, outputStream);
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

  for(char lineBuffer[2048] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    unsigned bounds[2] = {UINT_MAX, 0};
    unsigned skyline[10000] = {0};

    for(const char* cursor = lineBuffer; *cursor && *cursor != '\n';)
    {
      const unsigned left   = extractUint(&cursor),
                     height = extractUint(&cursor),
                     right  = extractUint(&cursor);

      // As per the problem statement.
      assert(height >= 001 && height <= 100);
      assert(left   <= 1e4 && right  <= 1e4);

      #define MIN(A, B) ((A) < (B) ? (A) : (B))
      #define MAX(A, B) ((A) > (B) ? (A) : (B))

      bounds[0] = MIN(bounds[0],  left);
      bounds[1] = MAX(bounds[1], right);

      // Trace the current building.
      for(unsigned i = left; i < right; ++i)
      {
        skyline[i] = MAX(skyline[i], height);
      }

      // Right face. Either overlapping or terminal.
      skyline[right] = MAX(skyline[right], 0);

      #undef MAX
      #undef MIN
    }

    dumpSkyline(skyline, bounds[0], bounds[1], stdout);
  }

  // The CRT takes care of cleanup.
}
