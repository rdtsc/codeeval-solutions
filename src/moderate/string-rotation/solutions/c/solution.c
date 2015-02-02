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

  static char stdoutBuffer[512] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[512] = "", * line = lineBuffer;
      fgets(line, sizeof lineBuffer, inputStream);
      line = lineBuffer)
  {
    // Trim trailing whitespace.
    for(char* c = line + strlen(line) - 1; *c && isspace(*c); *c-- = '\0');

    if(!*line) continue;

    /*
       lhs   rhs
        |     |
      Hello,lloHe

      lhs+lhs = HelloHello
                  \___/
                    |
                   rhs
    */

    const size_t lineMidpoint = strlen(line) / 2;

    // Left and right chunks should be the same length.
    if(line[lineMidpoint] != ',')
    {
      puts("False");
      continue;
    }

    line[lineMidpoint] = '\0';

    char lhs[sizeof lineBuffer] = "";

    strcat(strcat(lhs, line), line);

    // Locate RHS in LHS+LHS.
    const char* const substring = strstr(lhs, line + lineMidpoint + 1);

    puts(substring == NULL ? "False" : "True");
  }

  // The CRT takes care of cleanup.
}
