#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// Individual cells appear to be 2 characters long at most -- typically single
// characters or natural numbers under 100.
typedef char Cell[3];

typedef struct
{
  size_t rows, cols;
  Cell data[24][24];
} Matrix2d;

typedef struct
{
  const size_t rows, cols;
  size_t x, y, track;
  unsigned direction : 2;
} Matrix2dSnakeIterator;

static bool matrix2dIsWellFormed(const Matrix2d* const matrix)
{
  for(size_t y = 0; y < matrix->rows; ++y)
  for(size_t x = 0; x < matrix->cols; ++x)
  {
    if(!*matrix->data[y][x]) return false;
  }

  return true;
}

static bool matrix2dUnserialize(Matrix2d* const matrix,
                                FILE* const inputStream)
{
  assert(matrix && inputStream && !ferror(inputStream));

  size_t rows = 0, cols = 0;

  // Fetch dimensions.
  if(fscanf(inputStream, "%zu;%zu;", &rows, &cols) != 2 || !rows || !cols)
  {
    return false;
  }

  assert(rows < (sizeof  matrix->data / sizeof  *matrix->data));
  assert(cols < (sizeof *matrix->data / sizeof **matrix->data));

  static char lineBuffer[512] = "";

  // Fetch serialized payload.
  if(!fgets(lineBuffer, sizeof lineBuffer, inputStream)) return false;

  assert(strlen(lineBuffer) >= 2);

  matrix->rows = rows;
  matrix->cols = cols;

  const char* cursor = lineBuffer;

  for(size_t y = 0; y < rows; ++y)
  for(size_t x = 0; x < cols; ++x)
  {
    // Eat leading garbage.
    while(*cursor == ' ') ++cursor;

    char* cell = matrix->data[y][x];

    // Copy the cell's payload.
    for(size_t i = 0; (i < sizeof(Cell) - 1); ++i)
    {
      const char c = (*cursor++);

      if(c == ' ' || c == '\n' || c == EOF) break;

      *cell++ = c;
    }

    *cell = '\0';
  }

  // Confirm matrix rectangularity.
  assert(matrix2dIsWellFormed(matrix));

  return true;
}

static void matrix2dSnakeIteratorAdvance(Matrix2dSnakeIterator* const i)
{
  assert(i && i->rows && i->cols);

  enum {Left, Down, Right, Up};

  /*
    Directions <- {Left = 0, Down = 1, Right = 2, Up = 3}

    Cardinality is 4, so going to use 2 bits for nice overflow semantics.
  */

  switch(i->direction)
  {
    #define REACT(Direction, Index, Offset, Point, Action) \
    case (Direction): {                                    \
      if(((Index) + (Offset)) == (Point)) {                \
        if(++i->direction == Left) ++i->track; (Action);   \
      } else (Index) += (Offset);                          \
    } break

    REACT(Left, i->x, +1, (i->cols - i->track), ++i->y);
    REACT(Down, i->y, +1, (i->rows - i->track), --i->x);

    REACT(Right, i->x, -1, (i->track - 1), --i->y);
    REACT(Up,    i->y, -1, (i->track - 0), ++i->x);

    #undef REACT
  }
}

// Dump cells in an outer->inner snake pattern starting from the upper-left
// corner of the supplied matrix.
static void matrix2dDumpSnakePath(const Matrix2d* const matrix,
                                  FILE* const outputStream)
{
  assert(matrix && outputStream && !ferror(outputStream));
  assert(matrix->rows && matrix->cols);

  Matrix2dSnakeIterator cell =
  {
    .rows = matrix->rows,
    .cols = matrix->cols
  };

  const size_t cellCount = (matrix->rows * matrix->cols);

  for(size_t i = 0; i < cellCount; ++i)
  {
    fputs(matrix->data[cell.y][cell.x], outputStream);
    matrix2dSnakeIteratorAdvance(&cell);
    fputc(' ', outputStream);
  }

  fputc('\n', outputStream);
}

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

  for(Matrix2d matrix = {0}; matrix2dUnserialize(&matrix, inputStream);)
  {
    matrix2dDumpSnakePath(&matrix, stdout);
  }

  // The CRT takes care of cleanup.
}
