#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// Maps an English word to a number using linear search.
static int_fast32_t wordToNumber(const char* const word)
{
  assert(word && strlen(word) >= 3 && strlen(word) <= 9);

  static const struct
  {
    const char* literal;
    uint_fast32_t value;
  } alphabet[][6] =
  {
    #ifdef V
    #error
    #endif

    #define V(literal, value) {#literal, value},

    // Group the literals by their string lengths to speed up search a little.

    {V(one, 1)  V(two, 2)  V(six, 6)  V(ten, 10)},

    {V(zero, 0) V(four, 4) V(five, 5) V(nine, 9)},

    {
      V(three, 3)  V(seven, 7)  V(eight, 8)
      V(forty, 40) V(fifty, 50) V(sixty, 60)
    },

    {
      V(eleven, 11) V(twelve, 12) V(twenty, 20)
      V(thirty, 30) V(eighty, 80) V(ninety, 90)
    },

    {V(fifteen, 15) V(sixteen, 16) V(seventy, 70)},

    {
      V(thirteen, 13) V(fourteen, 14)
      V(eighteen, 18) V(nineteen, 19)
    },

    {V(seventeen, 17)}

    #undef V
  };

  static const size_t alphabetRowWidth = sizeof *alphabet / sizeof **alphabet;

  const size_t i = strlen(word) - 3;

  for(size_t j = 0; j < alphabetRowWidth && alphabet[i][j].literal; ++j)
  {
    if(!strcmp(alphabet[i][j].literal, word)) return alphabet[i][j].value;
  }

  return -1;
}

// Converts numbers written in English words that fall within the (-1e9, 1e9]
// interval into integers.
static int_fast32_t interpretNumber(char* words)
{
  assert(words);

  int_fast32_t result = 0, chunkSum = 0;

  for(char* chunk = strtok(words, " \n"); chunk; chunk = strtok(NULL, " \n"))
  {
    const bool is1e2 = !strcmp(chunk, "hundred"),
               is1e3 = !is1e2 && !strcmp(chunk, "thousand"),
               is1e6 = !is1e2 && !is1e3 && !strcmp(chunk, "million");

    if(!is1e2 && !is1e3 && !is1e6)
    {
      const int_fast32_t value = wordToNumber(chunk);

      if(value >= 0) chunkSum += value;
    }

    else if(is1e2) chunkSum *= 100;

    else
    {
      const uint_fast32_t magnitude = is1e3 ? 1e3 :
                                      is1e6 ? 1e6 : 1;

      result += chunkSum * magnitude;
      chunkSum = 0;
    }
  }

  // Don't forget the last chunk.
  result += chunkSum;

  const bool isNegative = !strcmp(words, "negative");

  return (isNegative) ? (-result) : (result);
}

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

  for(char lineBuffer[256] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    printf("%" PRIdFAST32 "\n", interpretNumber(lineBuffer));
  }

  // The CRT takes care of cleanup.
}
