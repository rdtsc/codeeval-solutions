#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

static bool isBalanced(const char* const message)
{
  assert(message && *message);

  struct {int open, closed;} count = {0, 0};

  for(const char* c = message; *c && *c != '\n'; ++c)
  {
    // Are we part of a smiley?
    const bool noLeadingColon = (*(c > message ? (c - 1) : c) != ':');

    if(*c == ')')
    {
      if(noLeadingColon && (--count.open < 0)) return false;

      if(--count.closed < 0) count.closed = 0;
    }

    else if(*c == '(')
    {
      ++count.open;

      if(noLeadingColon) ++count.closed;
    }
  }

  return (!count.closed && count.open >= 0);
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

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    puts(isBalanced(lineBuffer) ? "YES" : "NO");
  }

  // The CRT takes care of cleanup.
}
