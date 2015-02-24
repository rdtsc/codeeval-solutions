#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  size_t length;
  char data[256];
} Line;

static inline int lengthiestOrdering(const void* const lhs,
                                     const void* const rhs)
{
  const size_t x = ((const Line*)lhs)->length,
               y = ((const Line*)rhs)->length;

  return (x > y) ? (-1) : (x < y);
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[2048] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  // Number of longest lines to return.
  size_t sliceSize = 0;

  fscanf(inputStream, "%zu", &sliceSize);

  Line lines[64] = {{0}};
  size_t lineCount = 0;

  while(fgets(lines[lineCount].data, sizeof lines->data, inputStream))
  {
    char* const line = lines[lineCount].data;
    size_t lineSize  = strlen(line);

    // Trim trailing newline, if applicable.
    if(lineSize && line[lineSize - 1] == '\n') line[--lineSize] = '\0';

    lines[lineCount++].length = lineSize;
  }

  assert(sliceSize <= lineCount);

  // Sort the lines by their lengths in descending order.
  qsort(lines, lineCount, sizeof(Line), lengthiestOrdering);

  // Ensure we return at most `lineCount` results.
  if(sliceSize > lineCount) sliceSize = lineCount;

  for(size_t i = 0; i < sliceSize; ++i)
  {
    puts(lines[i].data);
  }

  // The CRT takes care of cleanup.
}
