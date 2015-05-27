#include <array>
#include <cassert>
#include <cstddef>
#include <iostream>

template<std::size_t BoardWidth> class RouteCounter
{
  private: static_assert(BoardWidth == 4, "Should be 4, as per spec.");

  public: unsigned operator()()
  {
    return this->compute(0, 0);
  }

  private: unsigned compute(const int x, const int y)
  {
    unsigned routeCount = 0;

    const auto targetX = (BoardWidth - 1),
               targetY = (BoardWidth - 1);

    // We've reached our destination at the lower-right cell.
    if((std::size_t)x == targetX && (std::size_t)y == targetY)
    {
      return ++routeCount;
    }

    // Guard bounds.
    else if(x < 0 || y < 0 || (std::size_t)x >= BoardWidth ||
                              (std::size_t)y >= BoardWidth)
    {
      return routeCount;
    }

    // Unused cell.
    else if(!this->board[y][x])
    {
      Toggle restore(this->board[y][x]);

      const auto next = [&](const int xOffset, const int yOffset)
      {
        return this->compute(x + xOffset, y + yOffset);
      };

      // Proceed to horizontal and vertical neighbors.
      routeCount = next(+1, 0) + next(0, +1) +
                   next(-1, 0) + next(0, -1);
    }

    return routeCount;
  }

  private: std::array<std::array<bool, BoardWidth>, BoardWidth> board;

  private: class Toggle
  {
    public: explicit Toggle(bool& target) : target(target)
    {
      this->target = true;
    }

    public: ~Toggle() {this->target = false;}

    private: bool& target;
  };
};

int main()
{
  const auto routeCount = RouteCounter<4>()();

  std::cout << routeCount << '\n';
}
