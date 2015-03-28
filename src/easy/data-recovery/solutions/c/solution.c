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

  static char stdoutBuffer[4096] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);
      putchar('\n'))
  {
    char* c = lineBuffer, * words[64] = {c}, * orderedWords[64] = {0};
    size_t wordCount = 1;

    for(;*c; ++c) if(*c == ' ' || *c == ';')
    {
      const bool isBreakpoint = (*c == ';');

      *c++ = '\0';

      if(isBreakpoint) break;

      assert(wordCount < sizeof words / sizeof *words);
      words[wordCount++] = c;
    }

    /*
      The last word in the sequence is always missing a supplied index in
      the RHS chunk. For example, the words "Foo Bar Baz" may be transformed
      as follows:

              Source             Scrambled               Result
              ------             ---------               ------
          "Foo Bar Baz"   ->   "Baz Foo Bar"   ->   "Baz Foo Bar;3 1"
            1   2   3            3   1   2            3   1   x
                                                              |
                                                          *omitted*

      The index for the last word ("Bar") is always omitted in the input. We
      can reconstruct this index by:

        1. Summing all word indices (in effect, this is just the word count);
        2. Computing the sum of the arithmetic range [1, word count];
        3. Subtracting the latter from the former.
    */

    size_t indexSum = 0;

    for(size_t n = 0, i = 0;; ++c)
    {
      if(*c == ' ' || *c == '\n' || !*c)
      {
        orderedWords[n - 1] = words[i++];
        indexSum += n;
        n = 0;

        if(*c == '\n' || !*c) break;
      }

      else n = (n * 10) + (*c - '0');
    }

    const size_t expectedIndexSum = (wordCount * (1 + wordCount)) / 2;

    orderedWords[expectedIndexSum - indexSum - 1] = words[wordCount - 1];

    for(size_t i = 0; i < wordCount; ++i)
    {
      if(i) putchar(' ');

      fputs(orderedWords[i], stdout);
    }
  }

  // The CRT takes care of cleanup.
}
