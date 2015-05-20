#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Board
{
  public: Board() : width() {}

  public: unsigned getLongestUniqueChainLength() const
  {
    unsigned result = 0;

    for(std::size_t x = 0; x < this->width; ++x)
    for(std::size_t y = 0; y < this->width; ++y)
    {
      result = std::max(result, this->getChainLengthAt(x, y));
    }

    return result;
  }

  private: unsigned getChainLengthAt(const int x,
                                     const int y,
                                     std::uint_least32_t history = 0) const
  {
    unsigned chainLength = 0;

    const auto width = this->width;

    // Guard bounds.
    if(x < 0 || y < 0 || (std::size_t)x >= width || (std::size_t)y >= width)
    {
      return chainLength;
    }

    const auto letterIndex = (this->data[y][x] - 'a');

    // We've already encountered this letter type before.
    if(history & (1 << letterIndex)) return chainLength;

    // The current letter type should not be used again.
    history |= (1 << letterIndex);

    #define TRY_NEXT(xOffset, yOffset) do {                                   \
      const auto newX = x + (xOffset), newY = y + (yOffset);                  \
      const auto newLength = this->getChainLengthAt(newX, newY, history) + 1; \
      chainLength = std::max(chainLength, newLength);                         \
    } while(false)

    // Proceed to horizontal and vertical neighbors.
    TRY_NEXT(+1, 0); TRY_NEXT(0, +1);
    TRY_NEXT(-1, 0); TRY_NEXT(0, -1);

    #undef TRY_NEXT

    return chainLength;
  }

  public: void unserialize(const std::string& state)
  {
    assert(!state.empty());
    assert(std::all_of(state.cbegin(), state.cend(), ::islower));

    this->data.clear();

    const std::size_t boardWidth = this->width = ::sqrt(state.size());

    // As per the problem statement.
    assert(boardWidth >= 2 && boardWidth <= 6);

    for(auto i = decltype(boardWidth){}; i < boardWidth; ++i)
    {
      this->data.push_back(state.substr(i * boardWidth, boardWidth));
    }
  }

  private: friend std::istream& operator>>(std::istream& inputStream,
                                           Board& board)
  {
    std::string state;

    if(std::getline(inputStream, state)) board.unserialize(state);

    return inputStream;
  }

  private: std::vector<std::string> data;
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

  for(Board board; inputStream >> board;)
  {
    std::cout << board.getLongestUniqueChainLength() << '\n';
  }
}
