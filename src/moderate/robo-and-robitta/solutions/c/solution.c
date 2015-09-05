#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// Reusing the solution to the Spiral Printing (problem id 57) challenge.

typedef unsigned char Cell;

typedef struct
{
  size_t rows, cols;
  Cell data[51][51];
} Matrix;

typedef struct
{
  const size_t rows, cols;
  size_t x, y, track;
  unsigned direction : 2;
} MatrixSnakeIterator;

static bool matrixUnserialize(Matrix* const matrix,
                              FILE* const inputStream)
{
  assert(matrix && inputStream && !ferror(inputStream));

  size_t rows = 0, cols = 0, x = 0, y = 0;

  // Fetch dimensions and target position.
  if(fscanf(inputStream, "%zu x %zu | %zu %zu", &cols, &rows, &x, &y) != 4)
  {
    return false;
  }

  assert((x <= cols) && (y <= rows));
  assert((rows >= 2) && (cols >= 2));
  assert(rows < (sizeof  matrix->data / sizeof  *matrix->data));
  assert(cols < (sizeof *matrix->data / sizeof **matrix->data));

  // Reset cells.
  for(size_t i = 0; i < rows; ++i)
  {
    memset(&matrix->data[i], 0, sizeof(Cell) * cols);
  }

  matrix->rows = rows;
  matrix->cols = cols;

  // Mark the target. (1,1) is at the bottom left of the matrix.
  matrix->data[rows - y][x - 1] = 1;

  return true;
}

static void matrixSnakeIteratorAdvance(MatrixSnakeIterator* const i)
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

static unsigned matrixGetSnakeSum(const Matrix* const matrix)
{
  assert(matrix && (matrix->rows >= 2) && (matrix->cols >= 2));

  MatrixSnakeIterator cell =
  {
    .rows = matrix->rows,
    .cols = matrix->cols
  };

  const size_t cellCount = (matrix->rows * matrix->cols);

  unsigned result = 0;

  for(size_t i = 0; i < cellCount; ++i)
  {
    ++result;

    if(matrix->data[cell.y][cell.x]) break;

    matrixSnakeIteratorAdvance(&cell);
  }

  return result;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[256] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(Matrix matrix = {0}; matrixUnserialize(&matrix, inputStream);)
  {
    printf("%u\n", matrixGetSnakeSum(&matrix));
  }

  // The CRT takes care of cleanup.
}
