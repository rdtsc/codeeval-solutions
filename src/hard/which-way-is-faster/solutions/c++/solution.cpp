#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <locale>
#include <queue>
#include <string>
#include <type_traits>
#include <vector>

template<typename T> struct Cell
{
  Cell(const T& x, const T& y) : x(x), y(y) {}

  bool operator==(const Cell& other) const
  {
    return (this->x == other.x) && (this->y == other.y);
  }

  T x, y;
};

template<typename T> class World
{
  private: static_assert(std::is_signed<T>::value, "T must be a signed type.");

  public: unsigned getShortestRouteLength() const
  {
    assert((this->goals.size() == 2) && (this->ports.size() == 2));

    // As per the problem statement.
    static constexpr auto earthCellCost = 2,
                          waterCellCost = 1,
                          portCellCost  = 1;

    /*
              Trivial path consisting      Complex path consisting
                  of a single leg.           of legs: {a, b, c}.

              ┌───┐ ┌───┐ ┌───┐ ┌───┐      ┌───┐ ┌───┐ ┌───┐ ┌───┐
              │   │ │^^^│ │^^^│ │ F │      │   │ │^^^│ │^^^│ │ F │
              └───┘ └───┘ └───┘ └─┼─┘      └───┘ └───┘ └───┘ └─┼─┘
              ┌───┐ ┌───┐ ┌───┐ ┌─┼─┐      ┌───┐ ┌───┐ ┌───┐ ┌─┼─┐
              │   │ │ P │ │^^^│ │ │ │      │ ┌─┼─┼ P │ │^^^│ │ c │
              └───┘ └───┘ └───┘ └─┼─┘      └─┼─┘ └─┼─┘ └───┘ └─┼─┘
              ┌───┐ ┌───┐ ┌───┐ ┌─┼─┐      ┌─┼─┐ ┌─┼─┐ ┌───┐ ┌─┼─┐
              │   │ │   │ │   │ │[P]│      │ a │ │ └─┼─┼─b─┼─┼ P │
              └───┘ └───┘ └───┘ └─┼─┘      └─┼─┘ └───┘ └───┘ └───┘
              ┌───┐ ┌───┐ ┌───┐ ┌─┼─┐      ┌─┼─┐ ┌───┐ ┌───┐ ┌───┐
              │ S ┼─┼───┼─┼───┼─┼─┘ │      │ S │ │   │ │   │ │   │
              └───┘ └───┘ └───┘ └───┘      └───┘ └───┘ └───┘ └───┘

                           Order is nonconsequential.
    */

    const auto& goalA = this->goals[0], goalB = this->goals[1],
                portA = this->ports[0], portB = this->ports[1];

    const auto earth = this->getDistances(goalA),
               water = this->getDistances(portA);

    const auto trivialPathLength = (earth[goalB.y][goalB.x] * earthCellCost);

    const auto complexPathLength = [&]
    {
      // Goal A -> (Closest) Port.
      const auto legA = [&]
      {
        const auto lhs = earth[portA.y][portA.x],
                   rhs = earth[portB.y][portB.x];

        return (std::min(lhs, rhs) * earthCellCost);
      }();

      // Port A -> Port B.
      const auto legB = [&]
      {
        const auto lhs = water[portA.y][portA.x],
                   rhs = water[portB.y][portB.x];

        const auto delta = ((lhs > rhs) ? (lhs - rhs) : (rhs - lhs));

        return ((delta * waterCellCost) + (portCellCost * 2));
      }();

      // Goal B -> (Closest) Port.
      const auto legC = [&]
      {
        const auto earth = this->getDistances(goalB);

        const auto lhs = earth[portA.y][portA.x],
                   rhs = earth[portB.y][portB.x];

        return (std::min(lhs, rhs) * earthCellCost);
      }();

      return (legA + legB + legC);
    }();

    return std::min(trivialPathLength, complexPathLength);
  }

  private: std::vector<std::vector<T>> getDistances(const Cell<T>& source) const
  {
    const std::size_t worldWidth = this->cells.size();

    // Table representing cell distances to the source.
    auto result = [&]
    {
      std::vector<std::vector<T>> result(worldWidth,
                                         std::vector<T>(worldWidth));

      for(auto& slice : result) for(auto& cell : slice)
      {
        static const auto unreachable = static_cast<T>(-1);

        cell = unreachable;
      }

      // Seed distance. The source cell is 0 cells away from itself.
      result[source.y][source.x] = 0;

      return result;
    }();

    const auto withinBounds = [&](const int x, const int y)
    {
      // Top and left bounds.
      if((y < 0) || (x < 0)) return false;

      // Bottom and right bounds.
      if(static_cast<std::size_t>(y) >= worldWidth ||
         static_cast<std::size_t>(x) >= worldWidth)
      {
        return false;
      }

      return true;
    };

    const auto shouldVisit = [&](const int x, const int y)
    {
      // Guard bounds.
      if(!withinBounds(x, y)) return false;

      // Cell must be accessible and unvisited.
      if((this->cells[y][x] != '*') || (result[y][x] > 0)) return false;

      // Guard source.
      if(source == Cell<T>(x, y)) return false;

      return true;
    };

    static struct {int xOffset, yOffset;} const neighbors[] =
    {
      {0, -1}, {-1, 0}, // Up,   Right.
      {0, +1}, {+1, 0}  // Down, Left.
    };

    std::queue<Cell<T>> search;

    // Populate the distance table via BFS.
    for(search.emplace(source); !search.empty();)
    {
      const auto cell = search.front();

      search.pop();

      for(const auto& neighbor : neighbors)
      {
        const auto x = (cell.x + neighbor.xOffset),
                   y = (cell.y + neighbor.yOffset);

        if(shouldVisit(x, y))
        {
          // Immediate neighbors are one cell away from us.
          result[y][x] = (result[cell.y][cell.x] + 1);
          search.emplace(x, y);
        }
      }
    }

    return result;
  }

  private: friend std::istream& operator>>(std::istream& inputStream,
                                           World& self)
  {
    self.cells.clear();
    self.goals.clear();
    self.ports.clear();

    std::string slice;

    // Fetch first slice.
    if(!(inputStream >> slice)) return inputStream;

    self.cells.emplace_back(slice);

    const auto worldWidth = slice.size();

    // As per the problem statement.
    assert((worldWidth >= 4) && (worldWidth <= 12));

    // Fetch remaining slices.
    for(auto i = decltype(worldWidth){1}; i < worldWidth; ++i)
    {
      if(!(inputStream >> slice)) assert(false);

      self.cells.emplace_back(slice);
    }

    assert(std::all_of(self.cells.cbegin(), self.cells.cend(),
      [&](const std::string& row)
    {
      const auto isSquare = (row.size() == self.cells.front().size());

      const auto isWellFormed =
        (row.find_first_not_of("^SFP*") == std::string::npos);

      return (isSquare && isWellFormed);
    }));

    // Clean up the maze.
    for(std::size_t y = 0; y < worldWidth; ++y)
    for(std::size_t x = 0; x < worldWidth; ++x)
    {
      auto& cell = self.cells[y][x];

      if(cell == '^') continue;

      else if(cell == 'P')
      {
        cell = '*';
        self.ports.emplace_back(x, y);
      }

      else if((cell == 'S') || (cell == 'F'))
      {
        cell = '*';
        self.goals.emplace_back(x, y);
      }
    }

    assert((self.goals.size() == 2) && (self.ports.size() == 2));

    return inputStream;
  }

  private: std::vector<std::string> cells;
  private: std::vector<Cell<T>> goals, ports;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Add pipes to the list of stream delimiters.
  inputStream.imbue(std::locale(inputStream.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    categories['|'] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  for(World<char> world; inputStream >> world;)
  {
    std::cout << world.getShortestRouteLength() << '\n';
  }
}
