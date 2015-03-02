#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static void reverse(char** begin, char** end)
{
  assert(begin && end && end > begin);

  while(end > begin)
  {
    char* oldBegin = *begin;

    *begin++ = *end;
    *end-- = oldBegin;
  }
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[1024] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[1024] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    char* elements[128] = {lineBuffer};
    size_t elementCount = 1;

    // Cut element strip.
    for(char* c = lineBuffer; *c && *c != '\n'; ++c)
    {
      if(*c == ',' || *c == ';')
      {
        assert(elementCount < sizeof elements / sizeof *elements);

        *c++ = '\0';
        elements[elementCount++] = c;
      }
    }

    // We are to reverse the list K elements at a time.
    const size_t k = atoi(elements[--elementCount]);

    for(size_t i = 0; i < elementCount / k; ++i)
    {
      reverse(elements + i * k,
              elements + i * k + k - 1);
    }

    fputs(*elements, stdout);

    for(size_t i = 1; i < elementCount; ++i)
    {
      putchar(',');
      fputs(elements[i], stdout);
    }

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
