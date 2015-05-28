#include <assert.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[4096] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  // LUT for the last digit of A^N.
  static const uint_fast8_t digitLut[][4] =
  {
    /*
      By running:

      #!/usr/bin/env perl
      use v5.10;
      for(2..9) {say "\n[$_]"; for my $n (1..9) {say "$_^$n: @{[$_**$n]}"}}

      We can observe that the last digit of A^N is part of a cyclic pattern.
    */

    {2, 4, 8, 6}, // 2,  4,   8,   16,    32,     64, ...
    {3, 9, 7, 1}, // 3,  9,  27,   81,   243,    729, ...
    {4, 6},       // 4, 16,  64,  256,  1024,   4096, ...
    {5},          // 5, 25, 125,  625,  3125,  15625, ...
    {6},          // 6, 36, 216, 1296,  7776,  46656, ...
    {7, 9, 3, 1}, // 7, 49, 343, 2401, 16807, 117649, ...
    {8, 4, 2, 6}, // 8, 64, 512, 4096, 32768, 262144, ...
    {9, 1}        // 9, 81, 729, 6561, 59049, 531441, ...
  };

  uint_fast32_t a = 0; // Base.
  uint_fast64_t n = 0; // Exponent.

  static const char* const scanFormat = "%" SCNuFAST32 " " // A -> [2,    9]
                                        "%" SCNuFAST64;    // N -> [1, 1e12]

  while(fscanf(inputStream, scanFormat, &a, &n) == 2)
  {
    assert(a >= 2 && a <= 9);
    assert(n >= 1 && n <= 1000000000000ULL);

    uint_fast64_t digitCount[10] = {0};

    // Bases 4 and 9 have a repeating pattern length of 2 for the last digit of
    // A^N. All other bases in the range [2, 9] except for 5 and 6 have a
    // pattern length of 4.
    const size_t patternWidth = (a == 4 || a == 9) ? (2) : (4);

    // 5^N and 6^N always end in the base number.
    if(a == 5 || a == 6) digitCount[a] = n;

    else
    {
      const uint_fast64_t digitOccurances = n / patternWidth;

      // Record the number of times our pattern's digits occur.
      for(size_t i = 0; i < patternWidth; ++i)
      {
        digitCount[digitLut[a - 2][i]] = digitOccurances;

        // Take care of any remainder occurrences.
        if(i < (n % patternWidth)) ++digitCount[digitLut[a - 2][i]];
      }
    }

    #define F(index) ", " #index ": %" PRIuFAST64
    #define V(index) ,digitCount[(index)]

    // Unrolled print loop yields a small gain in performance.
    printf("0: 0" F(1) F(2) F(3) F(4) F(5) F(6) F(7) F(8) F(9)
           "\n"   V(1) V(2) V(3) V(4) V(5) V(6) V(7) V(8) V(9));

    #undef V
    #undef F
  }

  // The CRT takes care of cleanup.
}
