#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>

class SudokuGrid
{
  public: SudokuGrid() : gridSize(), initialized()
  {
    static_assert((std::numeric_limits<value_type>::digits > maxGridWidth),
                  "Underlying type must be wider.");
  }

  public: bool unserialize(const std::string& data)
  {
    this->clear();

    if(data.empty()) return false;

    std::istringstream tokenizer(data);

    std::size_t sideLength = 0;

    if(!(tokenizer >> sideLength)) return false;

    // Guaranteed to only be dealing with 4x4 or 9x9 grids.
    assert(sideLength == 4 || sideLength == 9);

    this->gridSize = sideLength;

    static constexpr auto ignoreLimit =
      std::numeric_limits<std::streamsize>::max();

    tokenizer.ignore(ignoreLimit, ';');

    for(std::size_t y = 0; y < this->gridSize; ++y)
    for(std::size_t x = 0; x < this->gridSize; ++x)
    {
      if(!(tokenizer >> this->grid[y][x])) return false;

      tokenizer.ignore(ignoreLimit, ',');
    }

    this->initialized = true;

    return true;
  }

  private: void clear()
  {
    this->gridSize = 0;
    this->initialized = false;

    for(auto& row : this->grid) row.fill(0);
  }

  public: bool isSolved() const
  {
    assert(this->gridSize);

    if(!this->initialized) return false;

    // Check for digit uniqueness in all rows, columns, and sub-grids.
    for(std::size_t i = 0; i < this->gridSize; ++i)
    {
      if(!this->isValidSlice(i) || !this->isValidSubGrid(i))
      {
        return false;
      }
    }

    return true;
  }

  private: bool isValidSlice(const std::size_t index) const
  {
    assert(index < this->gridSize);

    value_type rowHistory = 0,
               colHistory = 0;

    // Check row and column uniqueness at the current index.
    for(std::size_t i = 0; i < this->gridSize; ++i)
    {
      const auto rowDigit = (1 << grid[index][i]),
                 colDigit = (1 << grid[i][index]);

      if((rowHistory & rowDigit) || (colHistory & colDigit)) return false;

      rowHistory |= rowDigit;
      colHistory |= colDigit;
    }

    return true;
  }

  private: bool isValidSubGrid(const std::size_t index) const
  {
    assert(index < this->gridSize);

    const std::size_t subGridSize = std::sqrt(this->gridSize);

    const auto yOffset = (index / subGridSize) * subGridSize,
               xOffset = (index % subGridSize) * subGridSize;

    value_type history = 0;

    // Check digit uniqueness in current sub-grid.
    for(auto y = yOffset; y < (yOffset + subGridSize); ++y)
    for(auto x = xOffset; x < (xOffset + subGridSize); ++x)
    {
      const auto digit = (1 << grid[y][x]);

      if(history & digit) return false;

      history |= digit;
    }

    return true;
  }

  public: operator bool() const
  {
    return this->isSolved();
  }

  private: friend std::istream&
    operator>>(std::istream& inputStream, SudokuGrid& instance)
  {
    std::string data;

    if(!std::getline(inputStream, data)) return inputStream;

    if(!instance.unserialize(data))
    {
      throw std::runtime_error("Failed to unserialize Sudoku grid.");
    }

    return inputStream;
  }

  private: static constexpr std::size_t maxGridWidth = 9;

  private: using value_type = unsigned;
  private: using row_type = std::array<value_type, maxGridWidth>;
  private: using grid_type = std::array<row_type, maxGridWidth>;

  private: grid_type grid;
  private: std::size_t gridSize;
  private: bool initialized;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(SudokuGrid grid; inputStream >> grid;)
  {
    std::cout << (grid ? "True" : "False") << '\n';
  }
}
