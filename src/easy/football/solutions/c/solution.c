#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
  size_t count;
  unsigned ids[32];
} Value;

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

// Ditto regarding performance.
static void emit(const unsigned n, const char prefix)
{
  if(prefix) putchar(prefix);

  // One and two digit results appear to be most common.
  if(n < 100)
  {
    if(n >= 10) putchar('0' + (n / 10) % 10);

    putchar('0' + (n % 10));

    return;
  }

  #define THRESHOLD(Value) n < (Value) ? (Value) / 10

  const unsigned divisor = THRESHOLD(1e3) :
                           THRESHOLD(1e4) :
                           THRESHOLD(1e5) : 0;

  #undef THRESHOLD

  for(unsigned i = divisor; i; i /= 10)
  {
    putchar('0' + ((n / i) % 10));
  }
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[10240] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    unsigned country = 1;
    Value results[128] = {{0}};

    // Fetch each country's preferred teams.
    for(const char* chunk = strtok(lineBuffer, "|");
        chunk; chunk = strtok(NULL, "|"), ++country)
    {
      while(*chunk)
      {
        const unsigned team = extractUint(&chunk);

        assert(team < (sizeof results / sizeof *results));

        Value* const value = &results[team];

        assert(value->count < (sizeof value->ids / sizeof *value->ids));

        value->ids[value->count++] = country;
      }
    }

    // Dump results.
    for(size_t i = 0; i < (sizeof results / sizeof *results); ++i)
    {
      if(results[i].count)
      {
        emit(i, '\0');
        emit(*results[i].ids, ':');

        for(size_t j = 1; j < results[i].count; ++j)
        {
          emit(results[i].ids[j], ',');
        }

        fputs("; ", stdout);
      }
    }

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
