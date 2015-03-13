#include <assert.h>
#include <stdio.h>
#include <string.h>

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

  for(char lineBuffer[256] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    const char* penultimateWord = strtok(lineBuffer, " \n");

    for(const char* word = penultimateWord; word;)
    {
      const char* nextWord = strtok(NULL, " \n");

      if(nextWord) penultimateWord = word;

      word = nextWord;
    }

    puts(penultimateWord);
  }

  // The CRT takes care of cleanup.
}
