#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "munkres.h"

static size_t count(const char* const haystack, const char needle)
{
  assert(haystack && *haystack && needle);

  size_t result = 0;

  for(const char* c = haystack; *c; ++c) if(*c == needle)
  {
    ++result;
  }

  return result;
}

static void explode(char* const source,
                    const char* const delimiters,
                    const size_t capacity,
                    char* bucket[])
{
  assert(source && *source && delimiters && *delimiters && capacity && bucket);

  memset(bucket, 0, sizeof(char*) * capacity);

  size_t i = 0;

  for(char* chunk = strtok(source, delimiters); chunk;
    chunk = strtok(NULL, delimiters))
  {
    assert(i < capacity);

    bucket[i++] = chunk;
  }
}

static size_t getLetterCount(const char* const sequence)
{
  size_t result = 0;

  for(const char* c = sequence; *c; ++c) if(isalpha(*c)) ++result;

  return result;
}

static size_t getVowelCount(const char* const sequence)
{
  size_t result = 0;

  for(const char* c = sequence; *c; ++c) switch(tolower(*c))
  {
    case 'a': case 'e': case 'i':
    case 'o': case 'u': case 'y': ++result;
  }

  return result;
}

static float getSuitabilitySocre(const char* const customerName,
                                 const char* const productName)
{
  assert(customerName && *customerName && productName && *productName);
  assert(strlen(customerName) && strlen(productName));

  const size_t customerLetterCount = getLetterCount(customerName),
               productLetterCount  = getLetterCount(productName);

  float baseScore = 0.0f;

  {
    const size_t customerVowelCount = getVowelCount(customerName);

    // If the number of letters in the product's name is odd then the
    // "Suitability Score" is the number of consonants in the customer's name.
    if(productLetterCount & 1)
    {
      baseScore = (customerLetterCount - customerVowelCount);
    }

    // If the number of letters in the product's name is even then the
    // "Suitability Score" is the number of vowels in the customer's name
    // multiplied by one and one-half.
    else baseScore = (customerVowelCount * 1.5f);
  }

  float scoreMultiplier = 1.0f;

  {
    #define MIN(X, Y) ((X) < (Y) ? (X) : (Y))

    const size_t limit = MIN(customerLetterCount, productLetterCount);

    #undef MIN

    // If the number of letters in the product's name shares any common factors
    // (besides 1) with the number of letters in the customer's name then the
    // "Suitability Score" is multiplied by a factor of one and one-half.
    if(limit > 1) for(size_t i = 2; i <= limit; ++i)
    {
      if(!(customerLetterCount % i) && !(productLetterCount % i))
      {
        scoreMultiplier = 1.5f;
        break;
      }
    }
  }

  return (baseScore * scoreMultiplier);
}

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

  for(char lineBuffer[1024] = "";
    fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    char* lhs = strtok(lineBuffer, ";"),
        * rhs = strtok(NULL, "\n");

    if(!lhs || !rhs)
    {
      puts("0.00");
      continue;
    }

    size_t nameCount    = (count(lhs, ',') + 1),
           productCount = (count(rhs, ',') + 1);

    assert((nameCount < 64) && (productCount < 64));

    // VLAs yield a slightly better memory score on CodeEval than straight
    // [m,c]alloc. Shouldn't rely on this in C11 and onwards, though.
    char* names[nameCount];
    explode(lhs, ",", nameCount, names);

    char* products[productCount];
    explode(rhs, ",", productCount, products);

    #define MAX(X, Y) ((Y) < (X) ? (X) : (Y))

    const size_t matrixWidth = MAX(nameCount, productCount);

    #undef MAX

    // The scoring table has to be square.
    float scores[matrixWidth][matrixWidth];
    memset(scores, 0, sizeof scores);

    // Fill suitability scoring table.
    for(size_t i = 0; i < nameCount;    ++i)
    for(size_t j = 0; j < productCount; ++j)
    {
      scores[i][j] = getSuitabilitySocre(names[i], products[j]);
    }

    float maxOfferValue = 0.0f;

    {
      int storage[matrixWidth][matrixWidth];
      memset(storage, 0, sizeof storage);

      int* values[matrixWidth];
      memset(values, 0, sizeof values);

      for(size_t i = 0; i < matrixWidth; ++i)
      {
        for(size_t j = 0; j < matrixWidth; ++j)
        {
          // HACK: The vendor's library only works with integers. I'm too lazy
          // to change its interface, though, so this hack should do for now.
          storage[i][j] = (scores[i][j] * 1000);
        }

        values[i] = storage[i];
      }

      // NOTE: The caller is responsible for freeing this. This doesn't matter
      // on CodeEval, so not going to bother.
      int** filter = getFilterMatrix(values, matrixWidth, true);

      // Tally selected cells in the cost matrix.
      for(size_t i = 0; i < matrixWidth; ++i)
      for(size_t j = 0; j < matrixWidth; ++j)
      {
        if(filter[i][j])
        {
          maxOfferValue += scores[i][j];
          break;
        }
      }
    }

    printf("%.2f\n", maxOfferValue);
  }

  // The CRT takes care of cleanup.
}
