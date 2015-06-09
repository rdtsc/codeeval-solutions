#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <locale>
#include <vector>

enum class Move {Unknown, Up, Down, Left, Right};

static std::istream& operator>>(std::istream& inputStream, Move& move)
{
  char sniff = '\0';

  inputStream >> sniff;

  switch(sniff)
  {
    #define DETECT(Enumerator) \
      case (*#Enumerator): move = Move::Enumerator; break

    DETECT(Up);   DETECT(Down);
    DETECT(Left); DETECT(Right);

    #undef DETECT
  }

  static constexpr auto ignoreLimit =
    std::numeric_limits<std::streamsize>::max();

  return inputStream.ignore(ignoreLimit, ';');
}

class Board
{
  public: Board() : width() {}

  private: using row_type = std::vector<unsigned>;

  public: Board& perform(const Move& move)
  {
    assert(this->width);

    switch(move)
    {
      #define DETECT(Enumerator) case Move::Enumerator

      DETECT(Up):   DETECT(Down):  this->moveVertically(move);   break;
      DETECT(Left): DETECT(Right): this->moveHorizontally(move); break;

      #undef DETECT

      default: assert(false);
    }

    return (*this);
  }

  private: void moveVertically(const Move direction)
  {
    assert(this->width);
    assert(direction == Move::Up || direction == Move::Down);

    this->scratch.clear();

    for(std::size_t x = 0; x < this->width; ++x)
    {
      for(std::size_t y = 0; y < this->width; ++y)
      {
        const auto cell = this->cells[y][x];

        if(cell) this->scratch.push_back(cell);
      }

      // Non-empty column.
      if(!this->scratch.empty())
      {
        this->reduce(this->scratch, direction);

        // Empty this column.
        for(std::size_t y = 0; y < this->width; ++y) this->cells[y][x] = 0;

        if(direction == Move::Up)
        {
          std::size_t y = 0;

          for(const auto col : this->scratch) this->cells[y++][x] = col;
        }

        else
        {
          std::size_t y = (this->width - 1);

          const auto scratchEnd = this->scratch.crend();

          for(auto i = this->scratch.crbegin(); i != scratchEnd; ++i)
          {
            this->cells[y--][x] = *i;
          }
        }

        this->scratch.clear();
      }
    }
  }

  private: void moveHorizontally(const Move direction)
  {
    assert(this->width);
    assert(direction == Move::Left || direction == Move::Right);

    this->scratch.clear();

    for(auto& row : this->cells)
    {
      for(const auto cell : row) if(cell)
      {
        this->scratch.push_back(cell);
      }

      // Non-empty row.
      if(!this->scratch.empty())
      {
        this->reduce(this->scratch, direction);

        // Empty this row.
        std::fill(row.begin(), row.end(), 0);

        if(direction == Move::Left) std::copy(this->scratch.cbegin(),
                                              this->scratch.cend(),
                                              row.begin());

        else std::copy(this->scratch.crbegin(),
                       this->scratch.crend(),
                       row.rbegin());

        this->scratch.clear();
      }
    }
  }

  private: void reduce(Board::row_type& row, Move direction)
  {
    assert(this->width && (direction != Move::Unknown));

    // "Rotate" the row 90 degrees CCW, if required.
    direction = (direction == Move::Up)   ? (Move::Left)  :
                (direction == Move::Down) ? (Move::Right) :
                (direction);

    if(direction == Move::Left) this->compress(row, row.cbegin(), row.cend());

    else
    {
      this->compress(row, row.crbegin(), row.crend());

      std::reverse(row.begin(), row.end());
    }
  }

  private: template<class Iterator> void compress(Board::row_type& target,
                                                  const Iterator begin,
                                                  const Iterator end)
  {
    assert(this->width);

    row_type reducedRow;

    for(auto i = begin; i != end; ++i)
    {
      const auto next = (i + 1);

      // Adjacent duplicate.
      if((next != end) && (*i == *next))
      {
        reducedRow.push_back(*i++ * 2);
      }

      else reducedRow.push_back(*i);
    }

    reducedRow.swap(target);
  }

  private: friend std::istream& operator>>(std::istream& inputStream,
                                           Board& board)
  {
    board.cells.clear();
    board.width = 0;

    std::size_t boardWidth = 0;

    if(!(inputStream >> boardWidth)) return inputStream;

    // As per the problem statement.
    assert(boardWidth >= 4 && boardWidth <= 10);

    board.cells.resize(boardWidth, Board::row_type(boardWidth));

    // Have to perform boardWidth^2 reads.
    const std::size_t opCount = (boardWidth * boardWidth);

    auto row = board.cells.begin();

    for(auto i = decltype(opCount){}; i < opCount; ++i)
    {
      Board::row_type::value_type cell = 0;

      if(!(inputStream >> cell))
      {
        board.cells.clear();
        break;
      }

      // Values should be 2^n, as per the game's rules.
      assert(!(cell & (cell - 1)));

      const auto colIndex = (i % boardWidth);

      (*row)[colIndex] = cell;

      if((colIndex + 1) == boardWidth) ++row;
    }

    board.width = boardWidth;

    return inputStream;
  }

  private: friend std::ostream& operator<<(std::ostream& outputStream,
                                           const Board& board)
  {
    const auto rowsEnd = board.cells.cend();

    for(auto i = board.cells.cbegin(); i != rowsEnd;)
    {
      const auto colsBegin = i->cbegin(),
                 colsEnd   = i->cend();

      outputStream << *colsBegin;

      for(auto j = (colsBegin + 1); j != colsEnd; ++j)
      {
        outputStream << ' ' << *j;
      }

      // No trailing pipe after the last row.
      if(++i != rowsEnd) outputStream << '|';
    }

    return outputStream;
  }

  private: std::vector<row_type> cells;
  private: row_type scratch;
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

  // Add semicolons and pipes to the list of stream delimiters.
  inputStream.imbue(std::locale(inputStream.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    categories[';'] = categories['|'] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  Move move = Move::Unknown;
  Board board;

  while(inputStream >> move >> board)
  {
    std::cout << board.perform(move) << '\n';
  }
}
