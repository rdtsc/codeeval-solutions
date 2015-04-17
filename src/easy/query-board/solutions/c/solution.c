#include <assert.h>
#include <stddef.h>
#include <stdio.h>

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

  unsigned char board[256][256] = {{0}};

  static const size_t boardWidth = sizeof board / sizeof *board;

  for(char lineBuffer[64] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    #define SNIFF(Char) (lineBuffer[5] == (Char))

    enum
    {
      UNKNOWN, SET_ROW, SET_COL, QUERY_ROW, QUERY_COL
    } const command = SNIFF('w') ? SET_ROW   : // SetRo[w]
                      SNIFF('l') ? SET_COL   : // SetCo[l]
                      SNIFF('R') ? QUERY_ROW : // Query[R]ow
                      SNIFF('C') ? QUERY_COL : // Query[C]ol
                                   UNKNOWN;

    #undef SNIFF

    #define FOR_EACH_ROW(Index) \
      for(size_t (Index) = 0; (Index) < boardWidth; ++(Index))

    #define FOR_EACH_COL(Index) FOR_EACH_ROW(Index)

    switch(command)
    {
      case SET_ROW: case SET_COL:
      {
        size_t index = 0;
        unsigned char value = 0;

        // Eat the command chunk (e.g., += strlen("SetRow")) and read the
        // numeric values that follow.
        sscanf(lineBuffer + sizeof("SetCmd") - 1, "%zu %hhu", &index, &value);

        if(command == SET_ROW) FOR_EACH_ROW(i) board[index][i] = value;

        else FOR_EACH_COL(i) board[i][index] = value;
      }
      break;

      case QUERY_ROW: case QUERY_COL:
      {
        size_t index = 0;

        // Eat the command chunk (e.g., += strlen("QueryRow")) and read the
        // requested row/column index.
        sscanf(lineBuffer + sizeof("QueryCmd") - 1, "%zu", &index);

        unsigned queryResult = 0;

        if(command == QUERY_ROW) FOR_EACH_ROW(i) queryResult += board[index][i];

        else FOR_EACH_COL(i) queryResult += board[i][index];

        printf("%u\n", queryResult);
      }
      break;

      case UNKNOWN: assert(0 && "Unknown command."); break;
    }

    #undef FOR_EACH_COL
    #undef FOR_EACH_ROW
  }

  // The CRT takes care of cleanup.
}
