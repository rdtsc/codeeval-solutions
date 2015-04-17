#include <assert.h>
#include <stdio.h>

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

  struct {int x, y;} pos = {0}, dst = {0};

  while(fscanf(inputStream, "%d %d %d %d", &pos.x, &pos.y,
                                           &dst.x, &dst.y) == 4)
  {
    char bearing[3] = "", * out = bearing;

    if(pos.y != dst.y) *out++ = (pos.y > dst.y ? 'S' : 'N');
    if(pos.x != dst.x) *out++ = (pos.x > dst.x ? 'W' : 'E');

    // If nothing's written to the buffer, we're already at the destination.
    puts(out == bearing ? "here" : bearing);
  }

  // The CRT takes care of cleanup.
}
