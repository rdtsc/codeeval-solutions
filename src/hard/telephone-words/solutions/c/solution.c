#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

static bool isTelephone(const char* const string, const size_t lengthThreshold)
{
  assert(string && lengthThreshold);

  size_t digitCount = 0;

  for(const char* c = string; *c && *c != '\n'; ++c, ++digitCount)
  {
    if(!isdigit(*c)) return false;
  }

  return (digitCount >= lengthThreshold);
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[65536] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  static struct
  {
    const char* values;
    size_t size;
  } const keypad[] =
  {
    #define REGISTER(Values) {#Values, sizeof(#Values) - 1}

    [0] = REGISTER(0),
    [1] = REGISTER(1),
    [2] = REGISTER(abc),
    [3] = REGISTER(def),
    [4] = REGISTER(ghi),
    [5] = REGISTER(jkl),
    [6] = REGISTER(mno),
    [7] = REGISTER(pqrs),
    [8] = REGISTER(tuv),
    [9] = REGISTER(wxyz)

    #undef REGISTER
  };

  static const size_t telephoneLength = 7;

  for(char lineBuffer[32] = "", outputBuffer[32768] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    assert(isTelephone(lineBuffer, telephoneLength));

    // Phone numbers aren't too long and have a fixed length, so this hack
    // should be alright.

    #define NEXT(Index) for(size_t i##Index = 0; \
      i##Index < keypad[lineBuffer[(Index)] - '0'].size; ++i##Index)

    #define EMIT(Char) (*output++ = (Char))

    #define SAVE(Index) \
      EMIT(keypad[lineBuffer[(Index)] - '0'].values[i##Index])

    bool isFirstResult = true;
    char* output = outputBuffer;

    NEXT(0) NEXT(1) NEXT(2) NEXT(3) NEXT(4) NEXT(5) NEXT(6)
    {
      if(!isFirstResult) EMIT(',');
      else isFirstResult = false;

      SAVE(0); SAVE(1); SAVE(2); SAVE(3); SAVE(4); SAVE(5); SAVE(6);
    }

    // Terminate the output buffer.
    EMIT('\0');

    puts(outputBuffer);

    #undef SAVE
    #undef EMIT
    #undef NEXT
  }

  // The CRT takes care of cleanup.
}
