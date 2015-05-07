#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static inline unsigned getMonthNumber(const char* const abbreviation)
{
  assert(abbreviation && (strlen(abbreviation) == 3));

  char targetMonth[4] = "";

  {
    const char* cursor = abbreviation;

    for(size_t i = 0; i < sizeof targetMonth; ++i, ++cursor)
    {
      targetMonth[i] = tolower(*cursor);
    }
  }

  static const char* const calendar[] =
  {
    "jan", "feb", "mar", "apr", "may", "jun",
    "jul", "aug", "sep", "oct", "nov", "dec"
  };

  for(size_t i = 0; i < (sizeof calendar / sizeof *calendar); ++i)
  {
    if(!strcmp(calendar[i], targetMonth)) return (i + 1);
  }

  assert(false);

  return 0;
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

  for(char lineBuffer[1024] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    // Dates are guaranteed to fall inside of [1990-01-01, 2020-12-31].
    bool employedMonths[(2020 - 1990) * 12] = {false};

    struct
    {
      unsigned year;
      char month[4];
    } lhs = {0}, rhs = {0};

    // Yields a small gain in performance over sscanf and friends in CE's
    // environment due to naive parsing rules and lack of locale awareness on
    // our part.
    for(const char* c = lineBuffer; *c; ++c)
    {
      // Range separators.
      while(*c == ';' || *c == ' ') ++c;

      // End of sequence.
      if(*c == '\n' || !*c) break;

      #define READ_MONTH(Symbol) do {                              \
        while(*c == '-') ++c;                                      \
        for(size_t i = 0; i < 3; ++i, ++c) (Symbol).month[i] = *c; \
        assert(*c == ' ');                                         \
      } while(false)

      #define READ_YEAR(Symbol) do {                         \
        while(*c == ' ') ++c;                                \
        for((Symbol).year = 0; *c >= '0' && *c <= '9'; ++c)  \
          (Symbol).year = ((Symbol).year * 10) + (*c - '0'); \
        assert(*c == '-' || *c == ';' || *c == '\n' || !*c); \
      } while(false)

      READ_MONTH(lhs);
      READ_YEAR(lhs);

      READ_MONTH(rhs);
      READ_YEAR(rhs);

      #undef READ_YEAR
      #undef READ_MONTH

      assert((lhs.year >= 1990) && (rhs.year <= 2020));

      const size_t begin = (lhs.year - 1990) * 12 + getMonthNumber(lhs.month),
                   end   = (rhs.year - 1990) * 12 + getMonthNumber(rhs.month);

      assert(end >= begin);

      // Record the relevant months as work experience.
      memset(employedMonths + begin, true, (end - begin + 1));
    }

    unsigned experienceMonths = 0;

    for(size_t i = 0; i < (sizeof employedMonths / sizeof *employedMonths); ++i)
    {
      if(employedMonths[i]) ++experienceMonths;
    }

    // Total working experience in years.
    printf("%u\n", (experienceMonths / 12));
  }

  // The CRT takes care of cleanup.
}
