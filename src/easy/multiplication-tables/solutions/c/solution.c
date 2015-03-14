#include <stdio.h>

int main(void)
{
  static char stdoutBuffer[640] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  for(unsigned x = 1; x <= 12; ++x)
  {
    printf("%u", x);

    for(unsigned y = 2; y <= 12; ++y) printf("%4u", x * y);

    putchar('\n');
  }
}
