#include <assert.h>
#include <stddef.h>
#include <stdio.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  // NOTE: This exceeds the C99-mandated 4,095 character limit for string
  // literals. Not portable but works with GCC without issues, though.
  static const char digits[] =
  {
    // perl -Mbignum=bpi -e 'print bpi(5000)'

    #include "digits.dat"
  };

  static const size_t digitCount = (sizeof digits - 1);

  for(size_t n = 0; fscanf(inputStream, "%zu", &n) == 1;)
  {
    assert(n && (n <= digitCount));
    putchar(digits[--n]);
    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
