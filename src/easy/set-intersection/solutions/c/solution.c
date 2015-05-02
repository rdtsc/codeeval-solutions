#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[512] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[1024] = "", * c = lineBuffer;
      fgets(lineBuffer, sizeof lineBuffer, inputStream);
      c = lineBuffer)
  {
    unsigned lhs[256] = {0},
             rhs[256] = {0};

    size_t lhsSize = 0,
           rhsSize = 0;

    // Get LHS. Yields a small gain in performance over sscanf and friends in
    // CE's environment due to naive parsing rules and lack of locale awareness
    // on our part.
    {
      unsigned n = 0;

      for(;;++c)
      {
        if(*c >= '0' && *c <= '9') n = (n * 10) + (*c - '0');

        else if(*c == ',' || *c == ';' || !*c)
        {
          assert(lhsSize < sizeof lhs / sizeof *lhs);
          assert(n >= lhs[lhsSize ? lhsSize - 1 : 0]);

          lhs[lhsSize++] = n;
          n = 0;

          if(*c == ';' || !*c) break;
        }
      }
    }

    // Get RHS.
    {
      unsigned n = 0;

      for(;;++c)
      {
        if(*c >= '0' && *c <= '9') n = (n * 10) + (*c - '0');

        else if(*c == ',' || *c == '\n' || !*c)
        {
          assert(rhsSize < sizeof rhs / sizeof *rhs);
          assert(n >= rhs[rhsSize ? rhsSize - 1 : 0]);

          rhs[rhsSize++] = n;
          n = 0;

          if(*c == '\n' || !*c) break;
        }
      }
    }

    // Both lists are guaranteed to be sorted.
    if(*rhs <= lhs[lhsSize - 1] &&
       *lhs <= rhs[rhsSize - 1])
    {
      bool isFirstTerm = true;

      for(size_t i = 0; i < lhsSize; ++i)
      for(size_t j = 0; j < rhsSize; ++j)
      {
        if(lhs[i] < rhs[j]) break;

        else if(lhs[i] == rhs[j])
        {
          if(!isFirstTerm) putchar(',');
          else isFirstTerm = false;

          printf("%u", lhs[i]);
          break;
        }
      }
    }

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
