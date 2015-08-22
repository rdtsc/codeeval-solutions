#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

static void interpret(const char* const instructions, FILE* const outputStream)
{
  assert(instructions && *instructions);
  assert(outputStream && !ferror(outputStream));

  char data[2048] = "";

  const size_t tapeLength = (sizeof data);

  #define STATIC_ASSERT(Expression, Note) \
    (void)sizeof(unsigned char[(Expression) ? 1 : -1])

  STATIC_ASSERT(tapeLength >= 64, "Tape length should be longer");

  STATIC_ASSERT(!(tapeLength & (tapeLength - 1)),
    "Tape length should be a power of 2.");

  #undef STATIC_ASSERT

  char* cell = &data[tapeLength / 2];

  // As per: https://en.wikipedia.org/wiki/Brainfuck#Commands
  for(const char* i = instructions; *i; ++i) switch(*i)
  {
    // Movement; unsafe.
    case '>': ++cell; break;
    case '<': --cell; break;

    // Mutation.
    case '+': ++*cell; break;
    case '-': --*cell; break;

    // Output.
    case '.': fputc(*cell, outputStream); break;

    // Flow control.
    case '[': if(!*cell) for(unsigned parity = 1; parity;)
    {
      switch(*++i)
      {
        case '[': ++parity; break;
        case ']': --parity; break;
      }
    }
    break;

    case ']': for(unsigned parity = 0;;)
    {
      switch(*i--)
      {
        case '[': ++parity; break;
        case ']': --parity; break;
      }

      if(!parity) break;
    }
    break;

    default: if(*i != '\n') assert(false);
  }

  fputc('\n', outputStream);
}

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

  for(char lineBuffer[2048] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    interpret(lineBuffer, stdout);
  }

  // The CRT takes care of cleanup.
}
