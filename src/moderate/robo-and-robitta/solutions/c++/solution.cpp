#include <cassert>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <locale>
#include <vector>

// Reusing the solution to the Spiral Printing (problem id 57) challenge.

template<typename T> class Route
{
  private: using row_type = std::vector<unsigned char>;

  public: Route() : field() {}

  public: T getValue() const
  {
    T result = 1;

    for(SnakeIterator i = *this; !*i; ++i) ++result;

    return result;
  }

  private: friend std::istream& operator>>(std::istream& inputStream,
                                           Route& route)
  {
    auto& field = route.field;

    unsigned x = 0, y = 0;

    // Fetch dimensions and target position.
    if(!(inputStream >> field.width >> field.height >> x >> y))
    {
      return inputStream;
    }

    assert((x <= field.width) && (y <= field.height));
    assert((field.width >= 2) && (field.height >= 2));

    // Reset field geometry.
    route.cells.clear();
    route.cells.resize(field.height, row_type(field.width));

    // Mark the target. (1,1) is at the bottom left of the matrix.
    route.cells[field.height - y][x - 1] = 1;

    return inputStream;
  }

  private: struct SnakeIterator
  {
    SnakeIterator(const Route& route) : data(route.cells), rows(data.size()),
      cols(data[0].size()), x(), y(), track(), direction()
    {
      assert(!route.cells.empty());
    }

    T operator*() const
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
        REACT(Up,    this->y, -1, (this->track + 0), ++this->x);

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

  private: struct {T width, height;} field;
  private: std::vector<row_type> cells;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Add x's and pipes to the list of stream delimiters.
  inputStream.imbue(std::locale(inputStream.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    for(const char c : "Xx|") if(c) categories[c] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  for(Route<unsigned> route; inputStream >> route;)
  {
    std::cout << route.getValue() << '\n';
  }
}
