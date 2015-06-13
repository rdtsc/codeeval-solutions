#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  int value;
  size_t index;
} Value;

typedef struct
{
  Value values[15][15];
  size_t width;
} Matrix;

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

static void matrixUnserialize(const char* state, Matrix* const matrix)
{
  assert(state && *state && matrix);

  matrix->width = 0;

  // Get the first row to work out the width of the matrix.
  while(*state != '|')
  {
    Value* const cell = &matrix->values[0][matrix->width];

    cell->value = extractInt(&state);
    cell->index = matrix->width++;
  }

  // As per the problem statement.
  assert(matrix->width && matrix->width <= 15);

  // Read subsequent rows based on the first row's dimensions.
  for(size_t y = 1; y < matrix->width; ++y)
  for(size_t x = 0; x < matrix->width; ++x)
  {
    Value* const cell = &matrix->values[y][x];

    cell->value = extractInt(&state);
    cell->index = x;
  }
}

static int matrixColumnOrdering(const void* const lhs,
                                const void* const rhs,
                                void* const instance)
{
  assert(lhs && rhs && instance);

  const Value* const lhsCell = (const Value*)lhs,
             * const rhsCell = (const Value*)rhs;

  int x = lhsCell->value,
      y = rhsCell->value;

  if(x == y)
  {
    const Matrix* const matrix = (const Matrix*)instance;

    // Columns below us break the tie.
    for(size_t i = 1; i < matrix->width; ++i)
    {
      x = matrix->values[i][lhsCell->index].value;
      y = matrix->values[i][rhsCell->index].value;

      if(x != y) break;
    }
  }

  return ((x < y) ? (-1) : (x > y));
}

static void matrixSort(Matrix* const matrix)
{
  assert(matrix && matrix->width);

  Value* const header = *matrix->values;

  // Sort first row in ascending order. In case of a tie, columns below the
  // two targets are to be taken into account. Non-standard GNU variant of
  // qsort used to avoid storing global state. When submitting to CodeEval,
  // _GNU_SOURCE should be defined prior to stdlib header inclusion.
  qsort_r(header, matrix->width, sizeof(Value), matrixColumnOrdering, matrix);

  // Rearrange the columns in the other rows based on the indices of the
  // (now sorted) first row.
  {
    Value sortedRow[sizeof *matrix->values / sizeof **matrix->values] = {{0}};

    for(size_t y = 1; y < matrix->width; ++y)
    {
      for(size_t i = 0; i < matrix->width; ++i)
      {
        sortedRow[i] = matrix->values[y][header[i].index];
      }

      memcpy(matrix->values[y], sortedRow, sizeof sortedRow);
    }
  }
}

// A bit faster than printf in this use case.
static inline size_t appendTo(char* buffer, int n)
{
  assert(buffer);

  const bool isNegative = (n < 0);

  if(isNegative) n = -n;

  const char* const bufferBegin = buffer;

  #define THRESHOLD(Value) n < (1e##Value) ? (1e##Value) / 10

  const unsigned divisor = THRESHOLD(1) : THRESHOLD(2) :
                           THRESHOLD(3) : THRESHOLD(4) :
                           THRESHOLD(5) : 0;

  #undef THRESHOLD

  if(isNegative) *buffer++ = '-';

  for(unsigned i = divisor; i; i /= 10)
  {
    *buffer++ = ('0' + (n / i) % 10);
  }

  return (buffer - bufferBegin);
}

static void matrixDump(const Matrix* const matrix, FILE* const outputStream)
{
  assert(matrix && matrix->width && outputStream && !ferror(outputStream));

  static char outputBuffer[1536] = "";
  char* out = outputBuffer;

  #define EMIT(Char) (*out++ = (Char))

  for(size_t y = 0; y < matrix->width;)
  {
    for(size_t x = 0; x < matrix->width; ++x)
    {
      assert((size_t)(out - outputBuffer) < sizeof outputBuffer);

      out += appendTo(out, matrix->values[y][x].value);

      EMIT(' ');
    }

    if(++y < matrix->width)
    {
      EMIT('|');
      EMIT(' ');
    }
  }

  EMIT('\n');
  EMIT('\0');

  #undef EMIT

  fputs(outputBuffer, outputStream);
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[16384] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[1536] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    Matrix matrix = {.width = 0};

    matrixUnserialize(lineBuffer, &matrix);
    matrixSort(&matrix);
    matrixDump(&matrix, stdout);
  }

  // The CRT takes care of cleanup.
}
