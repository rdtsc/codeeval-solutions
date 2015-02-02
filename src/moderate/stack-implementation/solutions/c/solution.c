#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct StringStack
{
  size_t size;
  char* data[128];
} StringStack;

static bool stackPush(StringStack* const stack, char* const value)
{
  assert(stack && value);

  if(stack->size >= (sizeof stack->data / sizeof *stack->data))
  {
    return false;
  }

  stack->data[stack->size++] = value;

  return true;
}

static char* stackPop(StringStack* const stack)
{
  assert(stack);

  return (stack->size) ? (stack->data[--stack->size]) : (NULL);
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[4096] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    StringStack integers = {0};

    for(char* n = strtok(lineBuffer, " \n"); n; n = strtok(NULL, " \n"))
    {
      stackPush(&integers, n);
    }

    for(;integers.size; stackPop(&integers))
    {
      fputs(stackPop(&integers), stdout);
      putchar(' ');
    }

    putchar('\n');
  }

  // The CRT takes care of cleanup.
}
