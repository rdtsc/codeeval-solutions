#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>

typedef struct
{
  unsigned* data;
  size_t size;
} TriangleRow;

static inline void reduce(TriangleRow* const top, TriangleRow* const bottom)
{
  assert(top && bottom && (bottom->size - top->size == 1));

  const size_t topRowSize = top->size;

  for(size_t i = 0; i < topRowSize; ++i)
  {
    /*
      Top:      A  B  C  D
               / \
      Bottom: E   F  G  H  I
    */

    const unsigned leftPathSum  = top->data[i] + bottom->data[  i  ], // A + E
                   rightPathSum = top->data[i] + bottom->data[i + 1]; // A + F

    #define MAX(x, y) ((x) > (y) ? (x) : (y))

    top->data[i] = MAX(leftPathSum, rightPathSum);

    #undef MAX
  }
}

int main(void)
{
  unsigned triangle[] =
  {
    /*
      May as well take advantage of the fact that the input triangle is static.
      Preprocessing the triangle into an array of integral tokens should save
      some memory and increase speed which yields a higher score on CodeEval.
      Transform: / / -> ","
    */

    #include "triangle.dat"
  };

  static const size_t cellCount = sizeof triangle / sizeof *triangle;

  /*
    C: Cells
    R: Rows

        r(1 + r)                                        (1 + 8c)^0.5 - 1
    c = --------    ->    r^2 + r + 2c = 0    ->    r = ----------------
           2                                                   2
  */

  const size_t triangleHeight = (sqrt(1 + 8 * cellCount) - 1) / 2;

  // Patch each cell with the optimal local solution (computed against its
  // immediate neighbors from the row below), working our way from the bottom
  // row to the triangle's top vertex.
  for(size_t y = triangleHeight; y--;)
  {
    // Classic arithmetic sequence sum.
    #define SEQUENCE_SUM(n) (((n) * (1 + (n))) / 2)

    // Jump to the appropriate row pair.
    TriangleRow top =
    {
      .data = triangle + SEQUENCE_SUM(y ? (y - 1) : y),
      .size = y
    },

    bottom =
    {
      .data = triangle + SEQUENCE_SUM(y),
      .size = y + 1
    };

    reduce(&top, &bottom);

    #undef SEQUENCE_SUM
  }

  // We're at the top vertex.
  printf("%u\n", *triangle);
}
