#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct
{
  unsigned price;
  unsigned cash;
} Transaction;

static Transaction unserializeTransaction(const char* record)
{
  assert(record);

  Transaction result = {0};
  bool hasCents = false;

  // Everything to the left of the semicolon is part of the purchase price.
  for(;*record && *record != ';'; ++record)
  {
    if(isdigit(*record)) result.price = (result.price * 10) + (*record - '0');

    else if(*record == '.') hasCents = true;
  }

  // Convert whole dollar values (i.e., no fractional part) to cents.
  if(!hasCents) result.price *= 100;

  // Everything to the right of the semicolon is part of the cash figure.
  for(hasCents = false; *record && *record != '\n'; ++record)
  {
    if(isdigit(*record)) result.cash = (result.cash * 10) + (*record - '0');

    else if(*record == '.') hasCents = true;
  }

  // Convert whole dollar values (i.e., no fractional part) to cents.
  if(!hasCents) result.cash *= 100;

  return result;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[1024] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  static const unsigned denominationValues[] =
  {
    1, 5, 10, 25, 50, 100, 200, 500, 1000, 2000, 5000, 10000
  };

  static const char* const denominationNames[] =
  {
    "PENNY", "NICKEL", "DIME", "QUARTER", "HALF DOLLAR", "ONE",
    "TWO", "FIVE", "TEN", "TWENTY", "FIFTY", "ONE HUNDRED"
  };

  static const size_t denominationCount = sizeof denominationValues /
                                          sizeof *denominationValues;

  // Should really be a static assertion.
  assert(denominationCount == (sizeof denominationNames /
                               sizeof *denominationNames));

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    const Transaction transaction = unserializeTransaction(lineBuffer);

    if(transaction.price > transaction.cash) puts("ERROR");

    else if(transaction.price == transaction.cash) puts("ZERO");

    else
    {
      unsigned change = transaction.cash - transaction.price;

      // Fast-forward to one dollar's index, if applicable.
      for(size_t i = (change <= 100) ? (5) : (denominationCount - 1); change;)
      {
        if(i && denominationValues[i] > change) --i;

        else
        {
          change -= denominationValues[i];

          fputs(denominationNames[i], stdout);

          if(change) putchar(',');
        }
      }

      putchar('\n');
    }
  }

  // The CRT takes care of cleanup.
}
