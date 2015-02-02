#include <assert.h>
#include <stdio.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  // Jump to the end of the (non-binary) stream.
  fseek(inputStream, 0, SEEK_END);

  printf("%lu\n", ftell(inputStream));

  // The CRT takes care of cleanup.
}
