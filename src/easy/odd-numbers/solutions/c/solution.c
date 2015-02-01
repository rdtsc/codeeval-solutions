#include <stdio.h>

int main(void)
{
  static char stdoutBuffer[256] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  for(unsigned i = 1; i < 100; i += 2) printf("%u\n", i);
}
