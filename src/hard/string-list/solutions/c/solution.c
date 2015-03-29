#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static inline int asciiOrdering(const void* const lhs, const void* const rhs)
{
  assert(lhs && rhs);

  register const char x = *(const char*)lhs,
                      y = *(const char*)rhs;

  return (x < y) ? (-1) : (x > y);
}

static inline void dumpCombinations(const size_t length,
                                    const char* const alphabet,
                                    const size_t uniqueLetterCount)
{
  assert(length && alphabet && uniqueLetterCount);

  size_t mask[10] = {0};

  assert(length < sizeof mask / sizeof *mask);

  for(;;)
  {
    for(size_t i = 0; i < length; ++i) putchar(alphabet[mask[i]]);

    // Create an index mask in the appropriate base.
    for(long i = length - 1;; --i)
    {
      if(i < 0) return;

      else if(++mask[i] == uniqueLetterCount) mask[i] = 0;

      else
      {
        putchar(',');
        break;
      }
    }
  }
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[51200] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[64] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);
      putchar('\n'))
  {
    char* chunk = lineBuffer;
    size_t requestedLength = 0;

    // Fetch requested length.
    {
      while(*chunk >= '0' && *chunk <= '9')
      {
        requestedLength = (requestedLength * 10) + (*chunk++ - '0');
      }

      assert(*chunk == ',');
    }

    size_t alphabetLength = 0;

    // Get the actual length of the supplied alphabet.
    for(const char* c = ++chunk; *c && *c != '\n'; ++c) ++alphabetLength;

    qsort(chunk, alphabetLength, 1, asciiOrdering);

    char alphabet[16] = "";
    size_t uniqueLetterCount = 0;

    // Get the unique elements in the sorted alphabet.
    {
      char cLeft = '\0';

      for(const char* c = chunk; *c && *c != '\n'; ++c) if(*c != cLeft)
      {
        // Skip immediate neighboring duplicates.
        alphabet[uniqueLetterCount++] = cLeft = *c;
      }
    }

    dumpCombinations(requestedLength, alphabet, uniqueLetterCount);
  }

  // The CRT takes care of cleanup.
}
