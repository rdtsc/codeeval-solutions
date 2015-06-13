#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>

template<typename T, std::size_t MaxWidth> class Matrix
{
  public: Matrix() : rows(), width() {}

  public: void sort()
  {
    assert(this->width);

    const auto rowBegin = this->rows.front().begin(),
               rowEnd   = std::next(rowBegin, this->width);

    // Sort first row in ascending order. In case of a tie, columns below the
    // two targets are to be taken into account.
    std::sort(rowBegin, rowEnd, [&](const Value& lhs, const Value& rhs)
    {
      if(lhs.value == rhs.value) return [&]
      {
        // Columns below us break the tie.
        for(std::size_t i = 1; i < this->width; ++i)
        {
          const auto leftValue  = this->rows[i][lhs.index].value,
                     rightValue = this->rows[i][rhs.index].value;

          if(leftValue != rightValue) return (leftValue < rightValue);
        }

        return false;
      }();

      return (lhs.value < rhs.value);
    });

    // Rearrange the columns in the other rows based on the indices of the
    // (now sorted) first row.
    {
      const auto rowsBegin = (this->rows.begin() + 1),
                 rowsEnd   = std::next(rowsBegin, this->width - 1);

      row_type sortedRow;

      const auto& header = this->rows.front();

      for(auto row = rowsBegin; row != rowsEnd; ++row)
      {
        for(std::size_t i = 0; i < this->width; ++i)
        {
          sortedRow[i] = (*row)[header[i].index];
        }

        sortedRow.swap(*row);
      }
    }
  }

  private: friend std::istream& operator>>(std::istream& inputStream,
                                           Matrix& matrix)
  {
    matrix.width = 0;

    Matrix::row_type row;
    std::size_t rowWidth = 0;

    static constexpr auto ignoreLimit =
      std::numeric_limits<std::streamsize>::max();

    // Get the first row to work out the width of the matrix.
    for(T value = 0; inputStream.peek() != '|'; ++rowWidth)
    {
      if(!(inputStream >> value)) return inputStream;

      row[rowWidth] = Value(value, rowWidth);

      inputStream.ignore(ignoreLimit, ' ');
    }

    // As per the problem statement.
    assert(rowWidth && rowWidth <= 15);

    row.swap(matrix.rows.front());

    // Read subsequent rows based on the first row's dimensions.
    for(std::size_t y = 1, x = 0; y < rowWidth; ++y, x = 0)
    {
      inputStream.ignore(ignoreLimit, '|');

      for(T value = 0; (x < rowWidth) && inputStream.peek() != '|'; ++x)
      {
        inputStream.ignore(ignoreLimit, ' ');

        if(!(inputStream >> value)) return inputStream;

        row[x] = Value(value, x);
      }

      row.swap(matrix.rows[y]);
    }

    matrix.width = rowWidth;

    return inputStream;
  }

  private: friend std::ostream& operator<<(std::ostream& outputStream,
                                           const Matrix& matrix)
  {
    for(std::size_t y = 0; y < matrix.width;)
    {
      for(std::size_t x = 0; x < matrix.width; ++x)
      {
        std::cout << matrix.rows[y][x].value << ' ';
      }

      if(++y != matrix.width) std::cout << "| ";
    }

    return outputStream;
  }

  private: struct Value
  {
    Value() : value(), index() {}

    Value(const T value, const std::size_t index) :
      value(value), index(index) {}

    T value;
    std::size_t index;
  };

  private: using row_type = std::array<Value, MaxWidth>;
  private: std::array<row_type, MaxWidth> rows;
  private: std::size_t width;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(Matrix<int, 15> matrix; inputStream >> matrix;)
  {
    matrix.sort();

    std::cout << matrix << '\n';
  }
}
