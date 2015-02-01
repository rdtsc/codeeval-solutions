#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define TERMINAL_ROWS 10
#define TERMINAL_COLS TERMINAL_ROWS

typedef struct
{
  size_t x, y;
  bool insertMode;
} TerminalCursor;

typedef struct
{
  char buffer[TERMINAL_ROWS + 1][TERMINAL_COLS + 1];
  TerminalCursor cursor;
} Terminal;

static void terminalRowTrimRightColumns(Terminal* const terminal)
{
  assert(terminal);

  const size_t currentRow = terminal->cursor.y;

  for(size_t i = terminal->cursor.x; i < TERMINAL_COLS; ++i)
  {
    terminal->buffer[currentRow][i] = ' ';
  }
}

static void terminalRowClear(Terminal* const terminal)
{
  assert(terminal);

  const size_t currentRow = terminal->cursor.y;

  for(size_t i = 0; i < TERMINAL_COLS; ++i)
  {
    terminal->buffer[currentRow][i] = ' ';
  }
}

static void terminalClearScreen(Terminal* const terminal)
{
  assert(terminal);

  const size_t originalRow = terminal->cursor.y;

  for(size_t i = 0; i < TERMINAL_ROWS; ++i)
  {
    terminal->cursor.y = i;
    terminalRowClear(terminal);
  }

  terminal->cursor.y = originalRow;
}

static void terminalPutChar(Terminal* const terminal, const char c)
{
  assert(terminal && isprint(c));

  char* const row = terminal->buffer[terminal->cursor.y];

  // Make room for a character at the current cursor position by shifting
  // everything to the right of it, working our way from right to left.
  // The last character is always discarded.
  if(terminal->cursor.insertMode)
  {
    for(size_t i = TERMINAL_COLS - 1; (i--) > terminal->cursor.x;)
    {
      row[i + 1] = row[i];
    }
  }

  row[terminal->cursor.x] = c;

  // Move cursor right.
  if(terminal->cursor.x < TERMINAL_COLS - 1) ++terminal->cursor.x;
}

static void terminalUnserializeCommands(Terminal* const terminal,
                                        const char* const commands)
{
  assert(terminal && commands);

  for(const char* c = commands; *c && *c != '\n'; ++c)
  {
    const char peek = *(c + 1);

    TerminalCursor* const cursor = &terminal->cursor;

    if(*c == '^' && ++c) switch(peek)
    {
      // Destruction.
      case 'c': terminalClearScreen(terminal);         break;
      case 'e': terminalRowTrimRightColumns(terminal); break;

      // Cursor positioning.
      case 'h': cursor->x = cursor->y = 0;                     break;
      case 'b': cursor->x = 0;                                 break;
      case 'r': if(cursor->x < TERMINAL_COLS - 1) ++cursor->x; break;
      case 'd': if(cursor->y < TERMINAL_ROWS - 1) ++cursor->y; break;
      case 'l': if(cursor->x) --cursor->x;                     break;
      case 'u': if(cursor->y) --cursor->y;                     break;

      // Cursor modes.
      case 'i': cursor->insertMode = true;  break;
      case 'o': cursor->insertMode = false; break;

      // Escaped characters.
      case '^': terminalPutChar(terminal, '^');

      // Explicit cursor positioning.
      default: if(*(c + 1) && isdigit(peek) && isdigit(*(c + 1)))
      {
        cursor->x = *++c - '0';
        cursor->y = peek - '0';
      }
    }

    else terminalPutChar(terminal, *c);
  }
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

  Terminal terminal = {.cursor = {0}};

  for(char lineBuffer[128] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    terminalUnserializeCommands(&terminal, lineBuffer);
  }

  for(size_t i = 0; i < TERMINAL_ROWS; ++i)
  {
    puts(terminal.buffer[i]);
  }

  // The CRT takes care of cleanup.
}
