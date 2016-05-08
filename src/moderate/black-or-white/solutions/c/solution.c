#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

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

  for(char lineBuffer[1024] = "";
    fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    const char* matrix[32] = {0};
    size_t matrixWidth = 0;

    for(const char* row = strtok(lineBuffer, "| \n");
      row; row = strtok(NULL, "| \n"))
    {
      assert(matrixWidth < (sizeof matrix / sizeof *matrix));

      matrix[matrixWidth++] = row;
    }

    // As per the problem statement.
    assert((matrixWidth >= 2) && (matrixWidth <= 25));

    struct {size_t width; unsigned sum;} result = {0};

    // Expand the (square) search submatrix until we find one where the sums
    // of all such submatrices are equal.
    for(size_t scope = 1; scope <= matrixWidth; ++scope)
    {
      bool isFirstSubmatrix = true;

      for(size_t yMajor = 0; yMajor <= (matrixWidth - scope); ++yMajor)
      for(size_t xMajor = 0; xMajor <= (matrixWidth - scope); ++xMajor)
      {
        unsigned cellSum = 0;

        for(size_t y = yMajor; y < (yMajor + scope); ++y)
        for(size_t x = xMajor; x < (xMajor + scope); ++x)
        {
          cellSum += (matrix[y][x] - '0');

          if(!isFirstSubmatrix && (cellSum > result.sum)) goto next;
        }

        if(isFirstSubmatrix)
        {
          result.sum = cellSum;
          isFirstSubmatrix = false;
        }

        else if(result.sum != cellSum) goto next;
      }

      result.width = scope;
      goto dump;

      next:;
    }

    dump: printf("%zux%zu, %u\n", result.width, result.width, result.sum);
  }

  // The CRT takes care of cleanup.
}
