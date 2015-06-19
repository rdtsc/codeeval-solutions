#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef uint_least32_t block_type;

static bool isUpperAlpha(const char* const string)
{
  assert(string && *string);

  for(const char* c = string; *c; ++c)
  {
    if((*c < 'A') || (*c > 'Z')) return false;
  }

  return true;
}

static block_type getBlockSnapshot(const char* const block)
{
  #define STATIC_ASSERT(Expression, Note) \
    (void)sizeof(unsigned char[(Expression) ? 1 : -1])

  STATIC_ASSERT((sizeof(block_type) * CHAR_BIT) >= 26,
    "Type must be wide enough to accommodate the English alphabet.");

  #undef STATIC_ASSERT

  assert(block && *block);

  block_type result = 0;

  #define SET_BIT_AT(Position) (result |= (1 << (Position)))

  for(const char* c = block; *c; ++c)
  {
    assert(*c >= 'A' && *c <= 'Z');

    SET_BIT_AT(*c - 'A');
  }

  #undef SET_BIT_AT

  return result;
}

static bool tryArrangement(block_type* const blocksBegin,
                           block_type* const blocksEnd,
                           const char* const word)
{
  assert(blocksBegin && blocksEnd && word && (blocksEnd >= blocksBegin));

  // Found.
  if(!*word) return true;

  // Exhausted.
  else if(blocksBegin == blocksEnd) return false;

  // Try next.
  for(block_type* i = blocksBegin; i != blocksEnd; ++i)
  {
    #define BIT_IS_SET_AT(Position) (*i & (1 << (Position)))

    const bool blockContainsLetter = BIT_IS_SET_AT(*word - 'A');

    #undef BIT_IS_SET_AT

    if(blockContainsLetter)
    {
      const block_type scratch = *i;

      *i = *blocksBegin;
      *blocksBegin = scratch;

      block_type* const nextBlock  = (blocksBegin + 1);
      const char* const nextLetter = (word + 1);

      const bool isFound = tryArrangement(nextBlock, blocksEnd, nextLetter);

      *blocksBegin = *i;
      *i = scratch;

      if(isFound) return true;
    }
  }

  return false;
}

static bool canArrange(const size_t blockCount,
                       block_type blocks[blockCount],
                       const char* const word)
{
  assert(blocks && word && strlen(word) && (blockCount >= strlen(word)));

  block_type* const blocksEnd = (blocks + blockCount);

  return tryArrangement(blocks, blocksEnd, word);
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
    char* cursor = lineBuffer;
    size_t blockCount = 0;

    // Get block count.
    {
      // Yields a small gain in performance over sscanf and friends in CE's
      // environment due to naive parsing rules and lack of locale awareness
      // on our part.
      for(;*cursor && *cursor != '|'; ++cursor)
      {
        if(*cursor >= '0' && *cursor <= '9')
        {
          blockCount = (blockCount * 10) + (*cursor - '0');
        }
      }

      assert(*cursor == '|');

      // Eat leading whitespace.
      for(++cursor; *cursor && *cursor == ' '; ++cursor);
    }

    const char* word = cursor;

    // Get target word.
    {
      for(;*cursor && *cursor != ' '; ++cursor);

      // Kill the delimiter.
      *cursor++ = '\0';

      assert(*cursor == '|');

      // Eat leading whitespace.
      for(++cursor; *cursor && *cursor == ' '; ++cursor);

      // As per the problem statement.
      assert(strlen(word) >= 3 && strlen(word) <= 18);
      assert(isUpperAlpha(word));
    }

    // Sanity check. Unrestricted VLAs can lead to UB if allocation fails.
    assert((blockCount <= 512) && (blockCount >= strlen(word)));

    // VLAs yield a slightly better memory score on CodeEval than straight
    // [m,c]alloc. Shouldn't rely on this in C11 and onwards, though.
    block_type blocks[blockCount];
    memset(blocks, 0, sizeof blocks);

    // Snapshot block face values.
    for(size_t i = 0; i < blockCount; ++i)
    {
      char* tail = strpbrk(cursor, " \n");

      if(tail)
      {
        // Kill the delimiter.
        *tail++ = '\0';

        // Blocks are cubic.
        assert(strlen(cursor) == 6);
        assert(isUpperAlpha(cursor));

        blocks[i] = getBlockSnapshot(cursor);
        cursor = tail;
      }
    }

    puts(canArrange(blockCount, blocks, word) ? "True" : "False");
  }

  // The CRT takes care of cleanup.
}
