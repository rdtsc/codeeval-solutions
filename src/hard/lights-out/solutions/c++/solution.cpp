#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

/*
                                          -1   P
                              P = !P  ->   0 P L P
                                           1   P
                                            -1 0 1

                   (1,1)                               (0,1)
        0 0 0      0 P 0      0 1 0    |    0 0 0      P 0 0      1 0 0
        0 0 0  ->  P 1 P  ->  1 1 1    |    0 0 0  ->  1 P 0  ->  1 1 0
        0 0 0      0 P 0      0 1 0    |    0 0 0      P 0 0      1 0 0

    1 1 0      0 0 0  \
    0 1 0  ->  0 0 0   >  via S = {(0,0), (1,0), (2,0), (0,1), (1,2), (2,2)}
    0 0 0      0 0 0  /

            (-,-)            (0,0)            (1,0)            (2,0)
        ┌───┬───┬───┐    ┌───┬───┬───┐    ┌───┬───┬───┐    ┌───┬───┬───┐
        │ ● │ ● │   │    │   │   │   │    │ ● │ ● │ ● │    │ ● │   │   │
        ├───┼───┼───┤    ├───┼───┼───┤    ├───┼───┼───┤    ├───┼───┼───┤
        │   │ ● │   │ -> │ ● │ ● │   │ -> │ ● │   │   │ -> │ ● │   │ ● │
        ├───┼───┼───┤    ├───┼───┼───┤    ├───┼───┼───┤    ├───┼───┼───┤
        │   │   │   │    │   │   │   │    │   │   │   │    │   │   │   │
        └───┴───┴───┘    └───┴───┴───┘    └───┴───┴───┘    └───┴─┬─┴───┘
                                                                 │
              ┌──────────────────────────────────────────────────┘
              │              (0,1)            (1,2)            (2,2)
        ┌───┬─┴─┬───┐    ┌───┬───┬───┐    ┌───┬───┬───┐    ┌───┬───┬───┐
        │ ● │   │   │    │   │   │   │    │   │   │   │    │   │   │   │
        ├───┼───┼───┤    ├───┼───┼───┤    ├───┼───┼───┤    ├───┼───┼───┤
        │ ● │   │ ● │ -> │   │ ● │ ● │ -> │   │   │ ● │ -> │   │   │   │
        ├───┼───┼───┤    ├───┼───┼───┤    ├───┼───┼───┤    ├───┼───┼───┤
        │   │   │   │    │ ● │   │   │    │   │ ● │ ● │    │   │   │   │
        └───┴───┴───┘    └───┴───┴───┘    └───┴───┴───┘    └───┴───┴───┘

                            P₀ P₁ P₂       L₀ L₁ L₂
                            P₃ P₄ P₅  <->  L₃ L₄ L₅
                            P₆ P₇ P₈       L₆ L₇ L₈

           L₀ = P₀+P₁+P₃       L₁ = P₀+P₁+P₂+P₄       L₂ = P₁+P₂+P₅
           L₃ = P₀+P₃+P₄+P₆    L₄ = P₁+P₃+P₄+P₅+P₇    L₅ = P₂+P₄+P₅+P₈
           L₆ = P₃+P₆+P₇       L₇ = P₄+P₆+P₇+P₈       L₈ = P₅+P₇+P₈
*/

class Game
{
  public: Game() : w(), h() {}

  public: int getMoveCount() const
  {
    assert(this->w && this->h && !this->cells.empty());

    static constexpr std::size_t sizeHint = 16;

    using value_type = std::pair<bool, std::vector<std::size_t>>;

    // TODO: Fix erroneous cases.
    auto lights = [&]
    {
      static struct {int xOffset, yOffset;} const neighbors[] =
      {
        {0, -1}, {-1, 0}, // Up,   Right.
        {0, +1}, {+1, 0}  // Down, Left.
      };

      const auto withinBounds = [&](const int x, const int y)
      {
        // Top and left bounds.
        if((y < 0) || (x < 0)) return false;

        // Bottom and right bounds.
        if(static_cast<decltype(this->h)>(y) >= this->h ||
           static_cast<decltype(this->w)>(x) >= this->w)
        {
          return false;
        }

        return true;
      };

      value_type entry;

      entry.second.reserve(sizeHint);

      std::vector<value_type> results;

      // Assign cell IDs.
      for(auto y = decltype(this->h){}; y < this->h; ++y)
      for(auto x = decltype(this->w){}; x < this->w; ++x)
      {
        entry.first = this->cells[y][x];

        entry.second.clear();
        entry.second.emplace_back((this->w * y) + x);

        for(const auto& neighbor : neighbors)
        {
          const auto newX = (x + neighbor.xOffset),
                     newY = (y + neighbor.yOffset);

          if(withinBounds(newX, newY))
          {
            entry.second.emplace_back((this->w * newY) + newX);
          }
        }

        // Turns out to be faster than using a set.
        std::sort(entry.second.begin(), entry.second.end());

        results.emplace_back(entry);
      }

      return results;
    }();

    assert(!lights.empty());

    const auto reduce = [&](const std::size_t i, const std::size_t j)
    {
      auto& lhs = lights[i],
          & rhs = lights[j];

      if(std::binary_search(rhs.second.cbegin(), rhs.second.cend(), i))
      {
        rhs.first ^= lhs.first;

        value_type::second_type swap;

        swap.reserve(sizeHint);

        const auto lhsBegin = lhs.second.cbegin(),
                   lhsEnd   = lhs.second.cend(),
                   rhsBegin = rhs.second.cbegin(),
                   rhsEnd   = rhs.second.cend();

        std::set_symmetric_difference(lhsBegin, lhsEnd, rhsBegin, rhsEnd,
          std::back_inserter(swap));

        rhs.second.swap(swap);
      }
    };

    const auto lightCount = lights.size();

    // Gaussian elimination.
    for(auto i = decltype(lightCount){}; i < lightCount; ++i)
    {
      const auto found = [&]
      {
        for(auto j = i; j < lightCount; ++j)
        {
          const auto& row = lights[j].second;

          if(std::binary_search(row.cbegin(), row.cend(), i))
          {
            std::swap(lights[i], lights[j]);

            return true;
          }
        }

        return false;
      }();

      if(!found) return -1;

      for(auto j = (i + 1); j < lightCount; ++j) reduce(i, j);
    }

    for(auto i = decltype(lightCount){}; i < lightCount; ++i)
    {
      for(auto j = decltype(i){}; j < i; ++j) reduce(i, j);
    }

    return std::count_if(lights.cbegin(), lights.cend(),
      [](const value_type& entry)
    {
      return entry.first;
    });
  }

  private: friend std::istream& operator>>(std::istream& inputStream,
                                           Game& self)
  {
    std::string payload;

    if(!(inputStream >> self.h >> self.w >> payload))
    {
      return inputStream;
    }

    assert(self.w && (self.w <= 10));
    assert(self.h && (self.h <= 10));

    assert(std::all_of(payload.cbegin(), payload.cend(), [](const char c)
    {
      return ((c == '.') || (c == 'O') || (c == '|'));
    }));

    assert(payload.size() == ((self.w * self.h) + (self.h - 1)));

    self.cells.clear();
    self.cells.resize(self.h, decltype(self.cells)::value_type(self.w));

    for(auto y = decltype(self.h){}; y < self.h; ++y)
    for(auto x = decltype(self.w){}; x < self.w; ++x)
    {
      const auto cell = payload[(y * (self.w + !!y)) + x];

      self.cells[y][x] = (cell == 'O');
    }

    return inputStream;
  }

  private: std::size_t w, h;
  private: std::vector<std::vector<bool>> cells;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(Game game; inputStream >> game;)
  {
    std::cout << game.getMoveCount() << '\n';
  }
}
