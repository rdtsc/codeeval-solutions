#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <locale>
#include <vector>

struct Variant
{
  Variant() : type(Letter), c() {}

  // Looks like we only have to support single characters and natural numbers.
  friend std::istream& operator>>(std::istream& inputStream, Variant& value)
  {
    // Eat leading garbage.
    while(std::isspace<char>(inputStream.peek(), inputStream.getloc()))
    {
      inputStream.ignore();
    }

    const auto peek = inputStream.peek();

    #define SIFT(Classifier, Type, Member) if((Classifier)(peek)) { \
      value.type = (Type); return (inputStream >> (value.Member));  \
    }

    SIFT(::isdigit, Number, u);
    SIFT(::isalpha, Letter, c);

    #undef SIFT

    return inputStream;
  }

  friend std::ostream& operator<<(std::ostream& outputStream,
                                  const Variant& value)
  {
    switch(value.type)
    {
      case Letter: return (outputStream << value.c);
      case Number: return (outputStream << value.u);
    }

    assert(false);

    return outputStream;
  }

  enum  {Letter, Number} type;
  union {char c; unsigned u;};
};

template<typename T> class Matrix2d
{
  public: using value_type = T;
  public: using row_type = std::vector<T>;

  private: friend std::istream& operator>>(std::istream& inputStream,
                                           Matrix2d& matrix)
  {
    std::size_t rows = 0, cols = 0;

    if(!(inputStream >> rows >> cols)) return inputStream;

    assert(rows && cols);

    row_type row;
    value_type value;

    matrix.data.clear();

    for(auto y = decltype(rows){}; y < rows; ++y)
    {
      row.clear();

      for(auto x = decltype(cols){}; x < cols; ++x)
      {
        if(!(inputStream >> value)) assert(false);

        row.emplace_back(value);
      }

      matrix.data.emplace_back(row);
    }

    return inputStream;
  }

  private: friend std::ostream& operator<<(std::ostream& outputStream,
                                           const Matrix2d& matrix)
  {
    const auto& data = matrix.data;

    const auto cellCount = (data.size() * data.front().size());

    // Confirm matrix rectangularity.
    assert(std::all_of(data.cbegin(), data.cend(), [&](const row_type& row)
    {
      return (row.size() == data.front().size());
    }));

    SnakeIterator cell(matrix);

    // Dump the cells in an outer->inner snake pattern starting from the
    // upper-left corner of the supplied matrix.
    for(auto i = decltype(cellCount){}; i < cellCount; ++i)
    {
      outputStream << (*cell++) << ' ';
    }

    return outputStream;
  }

  private: struct SnakeIterator
  {
    explicit SnakeIterator(const Matrix2d& matrix) : data(matrix.data),
      rows(data.size()), cols(data[0].size()), x(), y(), track(), direction()
    {
      assert(!matrix.data.empty());
    }

    const T& operator*() const
    {
      return this->data[this->y][this->x];
    }

    SnakeIterator& operator++()
    {
      enum {Left, Down, Right, Up};

      /*
        Directions <- {Left = 0, Down = 1, Right = 2, Up = 3}

        Cardinality is 4, so going to use 2 bits for nice overflow semantics.
      */

      static const auto digest = [&](std::size_t& index,
                                     const int increment,
                                     const std::size_t threshold,
                                     const std::function<void()> action)
      {
        if((index + increment) == threshold)
        {
          if(++this->direction == Left) ++this->track;

          action();
        }

        else index += increment;
      };

      switch(this->direction)
      {
        #define REACT(Direction, A, B, C, D) case (Direction): \
          digest((A), (B), (C), [&](){(D);}); break

        REACT(Left, this->x, +1, (this->cols - this->track), ++this->y);
        REACT(Down, this->y, +1, (this->rows - this->track), --this->x);

        REACT(Right, this->x, -1, (this->track - 1), --this->y);
        REACT(Up   , this->y, -1, (this->track + 0), ++this->x);

        #undef REACT
      }

      return (*this);
    }

    SnakeIterator operator++(int)
    {
      auto clone(*this);

      ++(*this);

      return clone;
    }

    const std::vector<row_type>& data;
    const std::size_t rows, cols;

    std::size_t x, y, track;
    unsigned direction : 2;
  };

  private: std::vector<row_type> data;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Add semicolons to the list of stream delimiters.
  inputStream.imbue(std::locale(inputStream.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    categories[';'] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  using input_iterator  = std::istream_iterator<Matrix2d<Variant>>;
  using output_iterator = std::ostream_iterator<input_iterator::value_type>;

  std::copy(input_iterator(inputStream), input_iterator(),
    output_iterator(std::cout, "\n"));
}
