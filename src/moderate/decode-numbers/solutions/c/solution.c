#include <assert.h>
#include <stdio.h>

static inline unsigned getDecodingCount(const char* message)
{
  assert(message);

  const char this = *message, peek = *(message + 1);
  if(!this || !peek || this == '\n' || peek == '\n') return 1;

  else if((this == '2' && peek >= '7') || this >= '3')
  {
    if(this == '2' && peek >= '7') ++message;

    return getDecodingCount(++message);
  }

  const unsigned lhs = getDecodingCount(++message),
                 rhs = getDecodingCount(++message);

  return (lhs + rhs);
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[128] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[64] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    printf("%u\n", getDecodingCount(lineBuffer));
  }

  // The CRT takes care of cleanup.
}
