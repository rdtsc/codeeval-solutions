#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Non-STDC.
#include <regex.h>

// NOTE: Exact-width types are not guaranteed to be available on all platforms.
typedef uint32_t ipv4_t;

typedef struct
{
  bool(* const isValidIpv4)(const char*);
  ipv4_t(* const getCompact)(const char*);
  const char* const pattern;
  regex_t spec;
} Ipv4Filter;

typedef struct
{
  ipv4_t ip;
  size_t count;
} Result;

#define MAKE_IPV4_BASE_CONVERTER(Name, Base)                          \
static ipv4_t ipv4GetCompactedBase##Name(const char* const address) { \
  assert(address && *address);                                        \
  return strtoul(address, NULL, (Base));                              \
}                                                                     \
static bool ipv4IsValidBase##Name(const char* const address) {        \
  assert(address && *address);                                        \
  const ipv4_t ip = ipv4GetCompactedBase##Name(address);              \
  return (ip >= (1U << 24) && ip <= ((1U << 31) * 2 - 2));            \
} struct ForceMacroTrailingSemicolon

#define MAKE_IPV4_DOTTED_CONVERTER(Name, Base)                        \
static ipv4_t ipv4GetCompactedDotted##Name(const char* address) {     \
  assert(address && *address);                                        \
  ipv4_t result = 0;                                                  \
  char* cursor = NULL;                                                \
  for(unsigned char shift = 4; shift--; address = ++cursor) {         \
    const unsigned octet = strtoul(address, &cursor, (Base));         \
    if((octet >= (1 << 8)) || (shift && (*cursor != '.'))) return 0;  \
    result += ((1 << (8 * shift)) * octet);                           \
  }                                                                   \
  return result;                                                      \
}                                                                     \
static bool ipv4IsValidDotted##Name(const char* const address) {      \
  assert(address && *address);                                        \
  const ipv4_t ip = ipv4GetCompactedDotted##Name(address);            \
  return (ip >= (1U << 24) && ip <= ((1U << 31) * 2 - 2));            \
} struct ForceMacroTrailingSemicolon

MAKE_IPV4_BASE_CONVERTER(Bin,  2);
MAKE_IPV4_BASE_CONVERTER(Oct,  8);
MAKE_IPV4_BASE_CONVERTER(Dec, 10);
MAKE_IPV4_BASE_CONVERTER(Hex, 16);

MAKE_IPV4_DOTTED_CONVERTER(Bin,  2);
MAKE_IPV4_DOTTED_CONVERTER(Oct,  8);
MAKE_IPV4_DOTTED_CONVERTER(Dec, 10);
MAKE_IPV4_DOTTED_CONVERTER(Hex, 16);

#undef MAKE_IPV4_DOTTED_CONVERTER
#undef MAKE_IPV4_BASE_CONVERTER

static int strictWeakOrdering(const void* const lhs,
                              const void* const rhs)
{
  assert(lhs && rhs);

  const size_t x = ((const Result*)lhs)->count,
               y = ((const Result*)rhs)->count;

  return (x < y) ? (-1) : (x > y);
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

  static Ipv4Filter filters[] =
  {
    #define FILTER_BASE(Name, Regex) {           \
      .isValidIpv4 = ipv4IsValidBase##Name,      \
      .getCompact  = ipv4GetCompactedBase##Name, \
      .pattern     = "(" Regex ")"               \
    }

    #define FILTER_DOTTED(Name, Regex) {                               \
      .isValidIpv4 = ipv4IsValidDotted##Name,                          \
      .getCompact  = ipv4GetCompactedDotted##Name,                     \
      .pattern     = "(" Regex "\\." Regex "\\." Regex "\\." Regex ")" \
    }

    FILTER_BASE(Bin, "[01]+"),
    FILTER_BASE(Oct, "0[0-7]+"),
    FILTER_BASE(Dec, "[0-9]+"),
    FILTER_BASE(Hex, "0x[0-9a-f]+"),

    FILTER_DOTTED(Bin, "[01]+"),
    FILTER_DOTTED(Oct, "0[0-9]+"),
    FILTER_DOTTED(Dec, "[0-9]+"),
    FILTER_DOTTED(Hex, "0x[0-9a-f]+")

    #undef FILTER_DOTTED
    #undef FILTER_BASE
  };

  static const size_t filterCount = (sizeof filters / sizeof *filters);

  // Compile filter patterns.
  for(size_t i = 0; i < filterCount; ++i)
  {
    static const int regexFlags = (REG_EXTENDED | REG_ICASE);

    Ipv4Filter* const filter = &filters[i];

    if(regcomp(&filter->spec, filter->pattern, regexFlags)) assert(false);
  }

  Result results[128] = {{0}};
  size_t resultCount = 0;

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    // Filter garbage.
    for(char* c = lineBuffer; *c; ++c)
    {
      if(!isxdigit(*c) && (*c != '.') && (*c != 'x')) *c = ' ';
    }

    // Isolate plausible candidates.
    for(const char* chunk = strtok(lineBuffer, " ");
        chunk; chunk = strtok(NULL, " "))
    {
      if(strlen(chunk) < (sizeof "0.0.0.0" - 1)) continue;

      // Detect and record IPs.
      for(size_t i = 0; i < filterCount; ++i)
      {
        const Ipv4Filter* const filter = &filters[i];

        regmatch_t group = {0};

        bool isFilterHit = false;

        // Try different filters until we find a match.
        for(const char* cursor = chunk;
            !regexec(&filter->spec, cursor, 1, &group, 0);
            ++cursor)
        {
          static char match[64] = "";

          memcpy(match, (cursor + group.rm_so), (group.rm_eo - group.rm_so));

          match[group.rm_eo - group.rm_so] = '\0';

          if(filter->isValidIpv4(match))
          {
            const ipv4_t ip = filter->getCompact(match);

            Result* result = NULL;

            // Find the corresponding entry in the result frequency table.
            // There should be few entries, so dumb linear search is fine.
            for(size_t j = 0; j < resultCount; ++j) if(results[j].ip == ip)
            {
              result = &results[j];
              break;
            }

            // Haven't encountered this IP before.
            if(!result)
            {
              assert(resultCount < (sizeof results / sizeof *results));

              result = &results[resultCount++];
            }

            result->ip = ip;
            ++result->count;

            isFilterHit = true;
            break;
          }
        }

        if(isFilterHit) break;
      }
    }
  }

  const Result* result = results;

  // Find the first most frequently occurring IP.
  for(size_t i = 1; i < resultCount; ++i)
  {
    if(results[i].count > result->count)
    {
      result = &results[i];
    }
  }

  size_t topCount = 0;

  // Find how many siblings, in terms of frequency, the most frequent IP has.
  for(size_t i = 0; i < resultCount; ++i)
  {
    if(results[i].count == result->count)
    {
      ++topCount;
    }
  }

  Result topIps[topCount];
  memset(topIps, 0, sizeof topIps);

  // Record siblings.
  for(size_t i = 0, found = 0; i < resultCount; ++i)
  {
    if(results[i].count == result->count)
    {
      topIps[found].ip = results[i].ip;
      topIps[found].count = results[i].count;

      if(++found == topCount) break;
    }
  }

  if(topCount > 1) qsort(topIps, topCount, sizeof *topIps, strictWeakOrdering);

  // Dump results.
  for(size_t i = 0; i < topCount; ++i)
  {
    #define OCTET(Offset) ((topIps[i].ip >> (24 - ((Offset) << 3))) & 0xFF)

    fprintf(stdout, "%u.%u.%u.%u ", OCTET(0), OCTET(1), OCTET(2), OCTET(3));

    #undef OCTET
  }

  putchar('\n');

  // The CRT takes care of cleanup.
}
