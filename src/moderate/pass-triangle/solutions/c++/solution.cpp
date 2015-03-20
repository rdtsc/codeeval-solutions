#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <iostream>

template<typename T> struct TriangleRow
{
  TriangleRow(T* const data, const std::size_t size) : data(data), size(size)
  {
    assert(this->data);
  }

  T* data;
  std::size_t size;
};

template<typename T> static inline void reduce(TriangleRow<T>& top,
                                               TriangleRow<T>& bottom)
{
  assert(top.data && bottom.data && (bottom.size - top.size == 1));

  const auto topRowSize = top.size;

  for(auto i = decltype(topRowSize){}; i < topRowSize; ++i)
  {
    /*
      Top:      A  B  C  D
               / \
      Bottom: E   F  G  H  I
    */

    const T leftPathSum  = top.data[i] + bottom.data[  i  ], // A + E
            rightPathSum = top.data[i] + bottom.data[i + 1]; // A + F

    top.data[i] = std::max(leftPathSum, rightPathSum);
  }
}

int main()
{
  using cell_type = unsigned;

  cell_type triangle[] =
  {
    /*
      May as well take advantage of the fact that the input triangle is static.
      Preprocessing the triangle into an array of integral tokens should save
      some memory and increase speed which yields a higher score on CodeEval.
      Transform: / / -> ","
    */

    #include "triangle.dat"
  };

  static const auto cellCount = sizeof triangle / sizeof *triangle;

  /*
    C: Cells
    R: Rows

        r(1 + r)                                        (1 + 8c)^0.5 - 1
    c = --------    ->    r^2 + r + 2c = 0    ->    r = ----------------
           2                                                   2
  */

  const std::size_t triangleHeight = (std::sqrt(1 + 8 * cellCount) - 1) / 2;

  auto sequenceSum = [](const unsigned n)
  {
    // Classic arithmetic sequence sum.
    return ((n * (1 + n)) / 2);
  };

  // Patch each cell with the optimal local solution (computed against its
  // immediate neighbors from the row below), working our way from the bottom
  // row to the triangle's top vertex.
  for(auto y = triangleHeight; y--;)
  {
    // Jump to the appropriate row pair.
    TriangleRow<cell_type> a(triangle + sequenceSum(y ? (y - 1) : y), y),
                           b(triangle + sequenceSum(y), y + 1);

    ::reduce(a, b);
  }

  // We're at the top vertex.
  std::cout << *triangle << '\n';
}
