#include <assert.h>
#include <stddef.h>
#include <stdio.h>

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

  for(char lineBuffer[10000] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    size_t linkMap[10001] = {0}, linkCount = 0;

    // Chain links look like "lhs-rhs" where the chunks match: (\d+|BEGIN|END)
    // A bit faster to parse the input by hand than via stdlib functions due to
    // naive parsing rules and lack of locale awareness on our part.
    for(const char* c = lineBuffer; *c && *c != '\n'; ++c)
    {
      size_t head = 0;

      // LHS is "BEGIN" or "END".
      if(*c == 'B' || *c == 'E')
      {
        head = (*c == 'E');
        while(*c++ != '-');
      }

      // LHS is an index.
      else while(*c != '-') head = (head * 10) + (*c++ - '0');

      while(*c == '-') ++c;

      size_t tail = 0;

      // RHS is "BEGIN" or "END";
      if(*c == 'B' || *c == 'E')
      {
        tail = (*c == 'E');
        while(*c && *c != ';' && *c != '\n') ++c;
      }

      // RHS is an index.
      else while(*c >= '0' && *c <= '9') tail = (tail * 10) + (*c++ - '0');

      assert(head < sizeof linkMap / sizeof *linkMap);
      assert(linkCount < sizeof linkMap / sizeof *linkMap);

      linkMap[head] = tail;
      ++linkCount;
    }

    // Visit each chain link, looking for repeats.
    for(size_t visitedLinkCount = 0, mapIndex = 0;
        visitedLinkCount <= linkCount; ++visitedLinkCount)
    {
      const size_t oldIndex = mapIndex;

      // We've either been here before, or we're at the terminal link.
      if(!linkMap[mapIndex])
      {
        if(mapIndex != 1 || visitedLinkCount != linkCount) puts("BAD");

        else puts("GOOD");

        break;
      }

      mapIndex = linkMap[mapIndex];

      // Mark this link as visited.
      linkMap[oldIndex] = 0;
    }
  }

  // The CRT takes care of cleanup.
}
