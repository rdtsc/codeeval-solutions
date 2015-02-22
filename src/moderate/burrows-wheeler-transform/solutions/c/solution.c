#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline int asciiOrdering(const void* const lhs, const void* const rhs)
{
  assert(lhs && rhs);

  register const char x = *(const char*)lhs,
                      y = *(const char*)rhs;

  return (x < y) ? (-1) : (x > y);
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

  for(char lineBuffer[1536] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    size_t lineLength = strlen(lineBuffer);

    for(char* c = &lineBuffer[lineLength - 1];
        *c && (*c == '\n' || *c == '|'); *c-- = '\0', --lineLength);

    // Kill the trailing pipe.
    lineBuffer[lineLength] = '\0';

    /*
      Encoded: "yyeep$-aass"; Last chars of all sorted source string rotations.
      Decoded: "easy-peasy$"; The '$' char always terminates the source string.

        #  RHS  =>  LHS (sort)  =>  Map LHS to RHS  =>  Index RHS via Mapping
        -  ---      ----------      --------------      ---------------------
        0   y           $                  5            RHS[ LHS[5] = 3 ] : e
        1   y           -                  6            RHS[ LHS[3] = 8 ] : a
        2   e           a                  7            RHS[ LHS[8] = A ] : s
        3   e           a                  8            RHS[ LHS[A] = 1 ] : y
        4   p           e                  2            RHS[ LHS[1] = 6 ] : -
        5   $           e                  3            RHS[ LHS[6] = 4 ] : p
        6   -           p                  4            RHS[ LHS[4] = 2 ] : e
        7   a           s                  9            RHS[ LHS[2] = 7 ] : a
        8   a           s                 10            RHS[ LHS[7] = 9 ] : s
        9   s           y                  0            RHS[ LHS[9] = 0 ] : y
        A   s           y                  1            RHS[ LHS[0] = 5 ] : $
    */

    char lhs[sizeof lineBuffer] = "", rhs[sizeof lineBuffer] = "";

    memcpy(lhs, lineBuffer, lineLength);
    memcpy(rhs, lineBuffer, lineLength);

    qsort(lhs, lineLength, 1, asciiOrdering);

    size_t mapping[sizeof lineBuffer] = {0};
    size_t mappingIndex = 0;

    for(size_t i = 0; i < lineLength; ++i)
    {
      size_t row = 0;

      for(;row < lineLength && lhs[i] != rhs[row]; ++row);

      if(lhs[i] == '$') mappingIndex = row;

      mapping[i] = row;
      rhs[row] = '\0';
    }

    for(size_t i = 0; i < lineLength; ++i)
    {
      mappingIndex = mapping[mappingIndex];
      rhs[i] = lineBuffer[mappingIndex];
    }

    puts(rhs);
  }

  // The CRT takes care of cleanup.
}
