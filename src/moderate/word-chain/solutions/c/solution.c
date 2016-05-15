#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
  size_t length;
  char data[64][2];
} Chain;

static Chain exclude(const char* const target, const Chain* const pool)
{
  assert(target && pool && pool->length);

  Chain result = {0};

  bool filtered = false;

  for(size_t i = 0; i < pool->length; ++i)
  {
    const char* const candidate = pool->data[i];

    const bool isTarget = (target[0] == candidate[0]) &&
                          (target[1] == candidate[1]);

    if(!isTarget || filtered)
    {
      assert(result.length < (sizeof pool->data / sizeof *pool->data));

      memcpy(result.data[result.length++], candidate, 2);
    }

    else if(isTarget) filtered = true;
  }

  return result;
}

static Chain getChain(const char* const word, const Chain* const pool)
{
  assert(word && pool && pool->length);

  Chain options = {0};

  for(size_t i = 0; i < pool->length; ++i)
  {
    if(word[1] == pool->data[i][0])
    {
      assert(options.length < (sizeof options.data / sizeof *options.data));

      memcpy(options.data[options.length++], pool->data[i], 2);
    }
  }

  if(!options.length) return (Chain)
  {
    .data   = {{word[0], word[1]}},
    .length = 1
  };

  Chain result = {0};

  for(size_t i = 0; i < options.length; ++i)
  {
    const Chain newPool  = exclude(options.data[i], pool),
                newChain = getChain(options.data[i], &newPool);

    if(newChain.length > result.length) result = newChain;
  }

  assert(result.length < (sizeof result.data / sizeof *result.data));

  memcpy(result.data[result.length++], word, 2);

  return result;
}

static size_t getLongestChainLength(const Chain* const pool)
{
  assert(pool && pool->length);

  size_t result = 1;

  for(size_t i = 0; i < pool->length; ++i)
  {
    const Chain newPool = exclude(pool->data[i], pool);

    const size_t chainLength = getChain(pool->data[i], &newPool).length;

    if(chainLength > result) result = chainLength;
  }

  return (result > 1 ? result : 0);
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

  for(char lineBuffer[512] = "";
    fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    Chain words = {0};

    for(char* word = strtok(lineBuffer, ",");
      word; word = strtok(NULL, ",\n"))
    {
      assert(words.length < (sizeof words.data / sizeof *words.data));

      words.data[words.length][0] = *word;
      words.data[words.length][1] = word[strlen(word) - 1];

      ++words.length;
    }

    const size_t chainLength = getLongestChainLength(&words);

    if(!chainLength) puts("None");

    else printf("%zu\n", chainLength);
  }

  // The CRT takes care of cleanup.
}
