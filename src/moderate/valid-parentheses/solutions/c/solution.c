#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static inline bool isBalancedParentheses(const char* const string)
{
  assert(string);

  struct
  {
    size_t size;
    char data[64];
  } stack = {0};

  assert(strlen(string) < sizeof stack.data);

  for(const char* c = string; *c && *c != '\n'; ++c) switch(*c)
  {
    case '(': case '{': case '[':
    {
      stack.data[stack.size++] = *c;
    }
    break;

    case ')': case '}': case ']':
    {
      // The '(' and ')' characters are directly adjacent to each other in the
      // ASCII table, while the "[]" and "{}" pairs have an extra character in
      // between.
      const size_t offset = ((*c == ')') ? 1 : 2);

      if(stack.size && (stack.data[stack.size - 1] + offset == (size_t)*c))
      {
        stack.data[--stack.size] = '\0';
      }

      else return false;
    }
  }

  return !stack.size;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[128] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[256] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    puts(isBalancedParentheses(lineBuffer) ? "True" : "False");
  }

  // The CRT takes care of cleanup.
}
