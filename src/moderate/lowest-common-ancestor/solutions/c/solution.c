#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

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

  static struct
  {
    unsigned value;
    size_t left, right;
  } const nodes[] =
  {
    #define NODE(Value, Left, Right) {(Value), (Left), (Right)}
    #define LEAF(Value) {(Value), 0, 0}

    [0] = NODE(30, 1, 2),                       /*      0    */
    [1] = NODE(8, 3, 4),                        /*     / \   */
    [2] = LEAF(52),                             /*    1   2  */
    [3] = LEAF(3),                              /*   / \     */
    [4] = NODE(20, 5, 6),                       /*  3   4    */
    [5] = LEAF(10),                             /*     / \   */
    [6] = LEAF(29)                              /*    5   6  */

    #undef LEAF
    #undef NODE
  };

  for(unsigned a = 0, b = 0; fscanf(inputStream, "%u %u", &a, &b) == 2;)
  {
    size_t i = 0;

    for(;;)
    {
      const unsigned nodeValue = nodes[i].value;

      if((a < nodeValue) && (b < nodeValue)) i = nodes[i].left;

      else if((a > nodeValue) && (b > nodeValue)) i = nodes[i].right;

      else break;
    }

    printf("%u\n", nodes[i].value);
  }

  // The CRT takes care of cleanup.
}
