#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static inline bool isUgly(const int_least64_t n)
{
  return (!(n % 2) || !(n % 3) || !(n % 5) || !(n % 7));
}

static int_least64_t evaluate(const char* const expression)
{
  static int_least64_t values[16] = {0};
  static char tokens[16] = {0};

  size_t valueCount = 0,
         tokenCount = 0;

  int_least64_t value = 0;

  for(const char* c = expression; *c; ++c)
  {
    if((*c >= '0') && (*c <= '9')) value = (value * 10) + (*c - '0');

    else if(*c == '+' || *c == '-')
    {
      assert(tokenCount < sizeof tokens);
      assert(valueCount < (sizeof values / sizeof *values));

      values[valueCount++] = value;
      tokens[tokenCount++] = *c;
      value = 0;
    }
  }

  assert(valueCount < (sizeof values / sizeof *values));

  values[valueCount++] = value;
  value = values[0];

  assert((tokenCount + 1) == valueCount);

  for(size_t i = 1, j = 0; i < valueCount; ++i) switch(tokens[j++])
  {
    case '+': value += values[i]; break;
    case '-': value -= values[i]; break;
  }

  return value;
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

  static const char operators[] = {'+', ' ', '-'};

  for(char lineBuffer[16] = "";
    fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    // Trim trailing whitespace.
    for(char* c = &lineBuffer[strlen(lineBuffer) - 1]; *c && isspace(*c);)
    {
      *c-- = '\0';
    }

    // Useful payload without left zero padding.
    const char* payload = lineBuffer;

    // Skip leading zeros to speed things up.
    while(*payload == '0') ++payload;

    // Literal consists of all zeros.
    if(!*payload) payload = lineBuffer;

    char line[32] = "";

    // Add spaces between digits to make room for operators; "123" -> "1 2 3".
    {
      char* out = line;

      for(const char* c = payload; *c; ++c)
      {
        *out++ = *c;
        *out++ = ' ';
      }

      *--out = '\0';
    }

    // There are floor(log10(n)) slots to insert operators.
    const size_t operatorCount = (strlen(payload) - 1);

    // There are 3^floor(log10(n)) possible expressions.
    const size_t expressionCount = pow(3, operatorCount);

    size_t select[16] = {0};
    size_t uglyExpressionCount = 0;

    for(size_t i = 0; i < expressionCount; ++i)
    {
      // Inject the selected operators.
      for(size_t j = 0, k = 1; j < operatorCount; ++j, k += 2)
      {
        line[k] = operators[select[j]];
      }

      // Select the next combination of operators.
      for(int j = (operatorCount - 1); j >= 0; --j)
      {
        if(++select[j] != sizeof operators) break;

        select[j] = 0;
      }

      uglyExpressionCount += isUgly(evaluate(line));
    }

    const unsigned paddingMultiplier =
      (unsigned)pow(3, (payload - lineBuffer));

    // Account for any expressions resulting from removed leading zeros.
    printf("%zu\n", uglyExpressionCount * paddingMultiplier);
  }

  // The CRT takes care of cleanup.
}
