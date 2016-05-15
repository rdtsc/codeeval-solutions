#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <type_traits>
#include <vector>

template<typename T> struct Point
{
  Point() : Point(0, 0) {}
  Point(const T x, const T y) : x(x), y(y) {}

  bool operator==(const Point& other) const
  {
    return (this->x == other.x) && (this->y == other.y);
  }

  T x, y;
};

template<typename T> class MazeSolver
{
  private: static_assert(std::is_signed<T>::value, "T must be a signed type.");

  public: MazeSolver(std::istream& inputStream)
  {
    inputStream >> *this;
  }

  private: void solve()
  {
    const std::size_t mazeWidth  = this->slices.front().size(),
                      mazeHeight = this->slices.size();

    static struct {int xOffset, yOffset;} const neighbors[] =
    {
      {0, -1}, {-1, 0}, // Up,   Right.
      {0, +1}, {+1, 0}  // Down, Left.
    };

    // Table representing cell distances to the source.
    auto distances = [&]
    {
      std::vector<std::vector<T>> result(mazeHeight,
                                         std::vector<T>(mazeWidth));

      for(auto& slice : result) for(auto& cell : slice)
      {
        static const auto unreachable = static_cast<T>(-1);

        cell = unreachable;
      }

      // Seed distance. The source cell is 0 cells away from itself.
      result[this->source.y][this->source.x] = 0;

      return result;
    }();

    const auto withinBounds = [&](const int x, const int y)
    {
      // Top and left bounds.
      if((y < 0) || (x < 0)) return false;

      // Bottom and right bounds.
      if(static_cast<std::size_t>(y) >= mazeHeight ||
         static_cast<std::size_t>(x) >= mazeWidth)
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
      if((this->slices[y][x] != ' ') || (distances[y][x] > 0))
      {
        return false;
      }

      return true;
    };

    std::queue<Cell> search;

    // Populate the distance table via BFS.
    for(search.emplace(this->source); !search.empty();)
    {
      const auto cell = search.front();

      search.pop();

      if(cell == this->target) break;

      for(const auto& neighbor : neighbors)
      {
        const auto x = (cell.x + neighbor.xOffset),
                   y = (cell.y + neighbor.yOffset);

        if(shouldVisit(x, y))
        {
          // Immediate neighbors are one cell away from us.
          distances[y][x] = (distances[cell.y][cell.x] + 1);
          search.emplace(x, y);
        }
      }
    }

    // Drain the queue for reuse during backtracking.
    while(!search.empty()) search.pop();

    // Mark the shortest path back to the source by following the smallest-
    // valued cells starting from the target.
    for(search.emplace(this->target); !search.empty();)
    {
      const auto cell = search.front();

      search.pop();

      const auto distanceToSource = distances[cell.y][cell.x];

      for(const auto& neighbor : neighbors)
      {
        const auto x = (cell.x + neighbor.xOffset),
                   y = (cell.y + neighbor.yOffset);

        // Found the next smallest neighboring cell.
        if(withinBounds(x, y) && distances[y][x] == (distanceToSource - 1))
        {
          this->slices[y][x] = '+';
          search.emplace(x, y);
          break;
        }
      }
    }

    // Mark the source and target as visited.
    this->slices[this->source.y][this->source.x] = '+';
    this->slices[this->target.y][this->target.x] = '+';
  }

  private: friend std::istream& operator>>(std::istream& inputStream,
                                           MazeSolver& maze)
  {
    auto& slices = maze.slices;

    slices.clear();

    for(std::string slice; std::getline(inputStream, slice);)
    {
      slices.emplace_back(slice);
    }

    // Sanity check maze slice contents and size.
    assert(std::all_of(slices.cbegin(), slices.cend(),
      [&](const std::string& slice)
    {
      const bool isValid =
        (slice.find_first_not_of(" *") == std::string::npos);

      const bool isRectangular = (slice.size() == slices.front().size());

      return (isValid && isRectangular);
    }));

    const auto& sourceSlice = slices.front(),
              & targetSlice = slices.back();

    // There should only be one source and target cell in the maze.
    assert([&]
    {
      const auto sourceCount =
        std::count(sourceSlice.cbegin(), sourceSlice.cend(), ' ');

      const auto targetCount =
        std::count(targetSlice.cbegin(), targetSlice.cend(), ' ');

      return (sourceCount == 1) && (targetCount == 1);
    }());

    // As per the problem statement.
    assert(slices.size() && (slices.size() <= 101));
    assert(slices.front().size() && (slices.front().size() <= 101));

    const T headSliceIndex = 0,
            tailSliceIndex = (slices.size() - 1);

    maze.source = {static_cast<T>(sourceSlice.find(' ')), headSliceIndex};
    maze.target = {static_cast<T>(targetSlice.find(' ')), tailSliceIndex};

    maze.solve();

    return inputStream;
  }

  private: friend std::ostream& operator<<(std::ostream& outputStream,
                                           const MazeSolver& maze)
  {
    for(const auto& slice : maze.slices)
    {
      outputStream << slice << '\n';
    }

    return outputStream;
  }

  private: using Cell = Point<T>;
  private: std::vector<std::string> slices;
  private: Cell source, target;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to, pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::cout << MazeSolver<short>(inputStream);
}
