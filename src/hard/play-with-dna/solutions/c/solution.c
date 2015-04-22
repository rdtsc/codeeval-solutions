#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  char* begin;
  size_t length;
  unsigned mismatch;
} Segment;

// Levenshtein distance.
static unsigned getPairMismatch(const char* const lhs,
                                const char* const rhs,
                                size_t length)
{
  assert(lhs && rhs && length);

  unsigned ops[++length];

  for(size_t i = 1; i < length; ++i)
  {
    ops[i] = i;
  }

  for(size_t i = 1; i < length; ++i)
  {
    *ops = i;

    unsigned prev = i - 1;

    for(size_t j = 1; j < length; ++j)
    {
      const unsigned del =  ops[  j  ] + 1,
                     ins =  ops[j - 1] + 1,
                     sub = (lhs[j - 1] != rhs[i - 1]) + prev;

      #define MIN(a, b, c) \
        ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

      const unsigned cache = ops[j];

      ops[j] = MIN(del, ins, sub);
      prev = cache;

      #undef MIN
    }
  }

  return ops[--length];
}

static int segmentOrdering(const void* const lhs, const void* const rhs)
{
  assert(lhs && rhs);

  const Segment* const x = (const Segment*)lhs,
               * const y = (const Segment*)rhs;

  assert(x->length == y->length);

  const size_t xMismatch = x->mismatch,
               yMismatch = y->mismatch;

  // Alpha order breaks the tie.
  if(xMismatch == yMismatch) return strncmp(x->begin, y->begin, x->length);

  return (xMismatch < yMismatch) ? (-1) : (xMismatch > yMismatch);
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[71680] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[1024] = "", outputBuffer[10240] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    char* segment = lineBuffer, * dna = lineBuffer;
    unsigned mismatch = 0;

    size_t segmentLength = 0, dnaLength = 0;

    for(char* c = segment; *c;)
    {
      // Get the target DNA segment.
      while(*c && *c != ' ') ++c;
      segmentLength = (c - lineBuffer);
      *c++ = '\0';

      // Get mismatch threshold.
      while(*c && *c >= '0' && *c <= '9')
      {
        mismatch = (mismatch * 10) + (*c++ - '0');
      }

      // Get the main DNA chunk.
      *c++ = '\0';
      for(dna = c; *c; ++c) if(*c == '\n') {*c = '\0'; break;}
      dnaLength = c - dna;
    }

    Segment segments[((dnaLength / segmentLength) + 1) * segmentLength];
    memset(segments, 0, sizeof segments);

    size_t segmentCount = 0;

    // Get the segments that satisfy the given mismatch threshold.
    for(size_t i = 0; i <= dnaLength - segmentLength; ++i)
    {
      char* const segmentBegin = dna + i;

      const unsigned localMismatch =
        getPairMismatch(segment, segmentBegin, segmentLength);

      if(localMismatch <= mismatch)
      {
        Segment* const segment = &segments[segmentCount];

        segment->begin = segmentBegin;
        segment->length = segmentLength;
        segment->mismatch = localMismatch;

        ++segmentCount;
      }
    }

    qsort(segments, segmentCount, sizeof(Segment), segmentOrdering);

    if(segmentCount)
    {
      char* out = outputBuffer;

      for(size_t i = 0; i < segmentCount; ++i, *out++ = ' ')
      for(size_t j = 0; j < segmentLength; ++j)
      {
        *out++ = segments[i].begin[j];
      }

      *out = '\0';
      fputs(outputBuffer, stdout);
      putchar('\n');
    }

    else puts("No match");
  }

  // The CRT takes care of cleanup.
}
