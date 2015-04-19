#include <assert.h>
#include <stddef.h>
#include <stdio.h>

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

  static struct
  {
    const char* symbol;
    unsigned value;
  } const numerals[] =
  {
    #define MAP(Symbol, Value) {#Symbol, (Value)}

    MAP(I ,   1), MAP(IV,   4), MAP(V ,   5), MAP(IX,   9),
    MAP(X ,  10), MAP(XL,  40), MAP(L ,  50), MAP(XC,  90),
    MAP(C , 1e2), MAP(CD, 4e2), MAP(D , 5e2), MAP(CM, 9e2),
    MAP(M , 1e3)

    #undef MAP
  };

  for(unsigned n = 0; fscanf(inputStream, "%u", &n) == 1;)
  {
    // Find the highest numeral that is <= N and subtract it from N,
    // repeating until N reaches zero.
    for(size_t i = sizeof numerals / sizeof *numerals; n;)
    for(size_t j = i; j--;) if(n >= numerals[j].value)
    {
      fputs(numerals[j].symbol, stdout);

      n -= numerals[j].value;
      i = j + 1;

      break;
    }

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
