#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

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

  for(char lineBuffer[256] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    float numbers[64] = {0};
    size_t numberCount = 0;

    // Process the expression RTL.
    for(const char* c = lineBuffer + strlen(lineBuffer) - 1; *c; --c)
    {
      if(*c == '\n') --c;

      if(isdigit(*c))
      {
        assert(numberCount < sizeof numbers / sizeof *numbers);

        numbers[numberCount++] = *c - '0';
      }

      else if(*c != ' ')
      {
        const float x = numbers[--numberCount], y = numbers[--numberCount];

        switch(*c)
        {
          case '*': numbers[numberCount++] = x * y; break;
          case '+': numbers[numberCount++] = x + y; break;
          case '/': numbers[numberCount++] = x / y; break;
          case '-': numbers[numberCount++] = x - y; break;
        }
      }
    }

    printf("%u\n", (unsigned)*numbers);
  }

  // The CRT takes care of cleanup.
}
