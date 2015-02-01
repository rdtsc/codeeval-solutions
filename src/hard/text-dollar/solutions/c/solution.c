#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>

static void printDigitGroup(const uint_fast32_t group)
{
  static const char* const alphabet[] =
  {
    // [0, 9] += 1
    "Zero", "One", "Two", "Three", "Four",
    "Five", "Six", "Seven", "Eight", "Nine",

    // [10, 19] += 1
    "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen",
    "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen",

    // [20, 90] += 10
    "Twenty", "Thirty", "Forty", "Fifty",
    "Sixty", "Seventy", "Eighty", "Ninety"
  };

  // Digit groups consist of 3 digits in the form of: ABC
  const size_t a = group / 100, bc = group % 100;

  if(a) printf("%sHundred", alphabet[a]);

  if(bc && bc <= 19) fputs(alphabet[bc], stdout);

  else if(bc >= 20)
  {
    fputs(alphabet[18 + (bc / 10)], stdout);

    const size_t c = bc % 10;

    if(c) fputs(alphabet[c], stdout);
  }
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[2048] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(uint_fast32_t n = 0; fscanf(inputStream, "%" SCNuFAST32, &n) == 1;)
  {
    static const char* const bases[] =
    {
      "", "Thousand", "Million", "Billion"
    };

    // Start from N's highest base.
    size_t baseIndex = log10(n) / 3;

    // Maximum of four 3-digit groups.
    uint_fast32_t digitGroups[4] = {0};
    size_t digitGroupCount = 0;

    // Split N into digit groups.
    for(;n; n /= 1000) digitGroups[digitGroupCount++] = n % 1000;

    // Print the groups in reverse, along with their base.
    for(size_t i = digitGroupCount; i--; --baseIndex)
    {
      printDigitGroup(digitGroups[i]);
      if(digitGroups[i]) fputs(bases[baseIndex], stdout);
    }

    puts("Dollars"); // Always pluarl.
  }

  // The CRT takes care of cleanup.
}
