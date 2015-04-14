#include <assert.h>
#include <stddef.h>
#include <stdio.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[2048] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  static struct
  {
    char symbol;
    size_t left, right;
  } const nodes[] =
  {
    #define NODE(Value, Left, Right) {(Value), (Left), (Right)}
    #define LEAF(Value) {(Value), 0, 0}

    // Dichotomic search table based on the Morse diagram found here:
    // https://en.wikipedia.org/wiki/Dichotomic_search

    // Root node and some unsupported characters.
    [ 0] = NODE(' ',  1,  2), [18] = NODE(' ', 37, 38),
    [20] = NODE(' ', 41, 42), [29] = NODE(' ', 59, 60),
    [30] = NODE(' ', 61, 62),

    // Letters.
    [ 4] = NODE('A',  9, 10), [23] = NODE('B', 47, 48),
    [25] = NODE('C', 51, 52), [11] = NODE('D', 23, 24),
    [ 1] = NODE('E',  3,  4), [17] = NODE('F', 35, 36),
    [13] = NODE('G', 27, 28), [15] = NODE('H', 31, 32),
    [ 3] = NODE('I',  7,  8), [22] = NODE('J', 45, 46),
    [12] = NODE('K', 25, 26), [19] = NODE('L', 39, 40),
    [ 5] = NODE('N', 11, 12), [ 6] = NODE('M', 13, 14),
    [14] = NODE('O', 29, 30), [21] = NODE('P', 43, 44),
    [28] = NODE('Q', 57, 58), [ 9] = NODE('R', 19, 20),
    [ 7] = NODE('S', 15, 16), [ 2] = NODE('T',  5,  6),
    [ 8] = NODE('U', 17, 18), [16] = NODE('V', 33, 34),
    [10] = NODE('W', 21, 22), [24] = NODE('X', 49, 50),
    [26] = NODE('Y', 53, 54), [27] = NODE('Z', 55, 56),

    // Numbers.
    [46] = LEAF('1'), [38] = LEAF('2'),
    [34] = LEAF('3'), [32] = LEAF('4'),
    [31] = LEAF('5'), [47] = LEAF('6'),
    [55] = LEAF('7'), [59] = LEAF('8'),
    [61] = LEAF('9'), [62] = LEAF('0')

    #undef LEAF
    #undef NODE
  };

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    size_t i = 0;
    char* output = lineBuffer;

    for(const char* c = lineBuffer;; ++c)
    {
      if(*c == '.') i = nodes[i].left;

      else if(*c == '-') i = nodes[i].right;

      else
      {
        assert(*c == ' ' || !*c || *c == '\n');

        *output++ = nodes[i].symbol;
        i = 0;

        if(!*c || *c == '\n')
        {
          *output = '\0';
          break;
        }
      }
    }

    puts(lineBuffer);
  }

  // The CRT takes care of cleanup.
}
