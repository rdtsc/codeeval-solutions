#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
  size_t width;
  int data[32][32];
} Matrix2d;

// Yields a small gain in performance over sscanf and friends in CE's
// environment due to naive parsing rules and lack of locale awareness
// on our part.
static int extractInt(const char** string)
{
  assert(string && *string);

  int result = 0;
  bool isNegative = false;
  const char* c = *string;

  #define IS_PAYLOAD() ((*c >= '0' && *c <= '9') || *c == '-')

  // Eat anything that's not a digit.
  while(*c && !IS_PAYLOAD()) ++c;

  for(;*c; ++c)
  {
    if(!IS_PAYLOAD())
    {
      ++c;
      break;
    }

    else if(*c == '-') isNegative = true;

    else result = (result * 10) + (*c - '0');
  }

  #undef IS_PAYLOAD

  *string = c;

  return (isNegative ? -result : result);
}

static void matrix2dUnserialize(Matrix2d* const matrix, FILE* const inputStream)
{
  assert(matrix && inputStream && !ferror(inputStream));

  for(char lineBuffer[256] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);
      ++matrix->width)
  {
    assert(matrix->width < (sizeof matrix->data / sizeof *matrix->data));

    size_t x = 0;

    for(const char* cursor = lineBuffer; *cursor;)
    {
      matrix->data[matrix->width][x++] = extractInt(&cursor);
    }
  }
}

static int getMaxArraySum(const int* const values, const size_t valueCount)
{
  assert(values && valueCount);

  int maxSum   = (*values),
      localMax = (*values);

  // Based on Kadane's algorithm.
  for(size_t i = 1; i < valueCount; ++i)
  {
    #define MAX(x, y) ((x) > (y) ? (x) : (y))

    localMax = MAX(values[i], localMax + values[i]);
    maxSum   = MAX(maxSum, localMax);

    #undef MAX
  }

  return maxSum;
}

static int matrix2dGetLargestSubmatrixSum(const Matrix2d* const matrix)
{
  assert(matrix && matrix->width);

  int result = 0,
      scratch[sizeof matrix->data / sizeof *matrix->data] = {0};

  // NOTE: Should probably rotate the matrix for better data locality.
  for(size_t i = 0; i < matrix->width; ++i)
  {
    memset(scratch, 0, (matrix->width * sizeof(int)));

    for(size_t x = i; x < matrix->width; ++x)
    {
      for(size_t y = 0; y < matrix->width; ++y)
      {
        scratch[y] += matrix->data[y][x];
      }

      const int maxRowSum = getMaxArraySum(scratch, matrix->width);

      if(maxRowSum > result) result = maxRowSum;
    }
  }

  return result;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  Matrix2d matrix = {0};

  #define STATIC_ASSERT(Expression) \
    (void)sizeof(unsigned char[(Expression) ? 1 : -1])

  // Matrix bounds should be square and at least 20 cells wide.
  STATIC_ASSERT((sizeof  matrix.data / sizeof  *matrix.data) ==
                (sizeof *matrix.data / sizeof **matrix.data) &&
                (sizeof  matrix.data / sizeof  *matrix.data) >= 20);

  #undef STATIC_ASSERT

  matrix2dUnserialize(&matrix, inputStream);

  printf("%d\n", matrix2dGetLargestSubmatrixSum(&matrix));

  // The CRT takes care of cleanup.
}
