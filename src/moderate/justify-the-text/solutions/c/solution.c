#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
  size_t width;
  const char* data;
} Word;

static void dumpWords(const Word* const words,
                      const size_t wordCount,
                      const size_t spaceCount,
                      size_t padding,
                      FILE* const outputStream)
{
  assert(words && wordCount && outputStream && !ferror(outputStream));

  static char lineBuffer[512] = "";

  char* cursor = lineBuffer;

  #define INJECT_WORD(Index) do {                              \
    memcpy(cursor, words[(Index)].data, words[(Index)].width); \
    cursor += words[(Index)].width;                            \
    *cursor = '\0';                                            \
  } while(false)

  #define INJECT_PADDING(Size) do { \
    memset(cursor, ' ', (Size));    \
    cursor += (Size);               \
  } while(false)

  INJECT_WORD(0);

  for(size_t i = 1; i < wordCount; ++i)
  {
    const bool shouldInjectPadding = !!(padding ? padding-- : false);
    const size_t totalWidth = (spaceCount + shouldInjectPadding);

    INJECT_PADDING(totalWidth);
    INJECT_WORD(i);
  }

  fputs(lineBuffer, outputStream);
  fputc('\n', outputStream);

  #undef INJECT_PADDING
  #undef INJECT_WORD
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[32768] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  static const size_t rulerColumn = 80;

  Word words[256] = {{0}};

  for(char lineBuffer[2048] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    size_t wordCount = 0,
           lineWidth = 0;

    for(const char* word = strtok(lineBuffer, " \n");
        word; word = strtok(NULL, " \n"), ++wordCount)
    {
      assert(wordCount < (sizeof words / sizeof *words));

      const size_t wordWidth = strlen(word);

      /*
        |foo bar baz bax     |
        |foo   bar   baz  bax|
            ^^^   ^^^   ^^
      */

      if((wordWidth + lineWidth) > rulerColumn)
      {
        const size_t margin     = (rulerColumn + 1 - lineWidth),
                     padding    = (margin % (wordCount - 1)),
                     spaceCount = (margin / (wordCount - 1) + 1);

        dumpWords(words, wordCount, spaceCount, padding, stdout);

        wordCount = lineWidth = 0;
      }

      words[wordCount].width = wordWidth;
      words[wordCount].data  = word;
      lineWidth += (wordWidth + 1);
    }

    dumpWords(words, wordCount, 1, 0, stdout);
  }

  // The CRT takes care of cleanup.
}
