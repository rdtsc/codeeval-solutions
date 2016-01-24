#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

template<typename T> struct Cell
{
  Cell() : Cell(0, 0) {}
  Cell(const T x, const T y, const T id = 0) : x(x), y(y), id(id) {}
  T x, y, id;
};

template<typename T> class Maze
{
  public: using Cell = ::Cell<T>;

  public: Maze(const std::string& data, const std::size_t width) : data(data),
                                                                   width(width)
  {
    static_assert(std::is_signed<T>::value, "T must be a signed type.");

    assert(!data.empty() && (data.size() == (width * width)));
  }

  public: std::vector<T> getPathLengths(const Cell& source,
                                        const std::vector<Cell>& targets) const
  {
    static struct {int xOffset, yOffset;} const neighbors[] =
    {
      {0, -1}, {-1, 0}, // Up,   Right.
      {0, +1}, {+1, 0}  // Down, Left.
    };

    // Table representing cell distances to the source.
    auto distances = [&]
    {
      std::vector<std::vector<T>> result(this->width,
                                         std::vector<T>(this->width));

      for(auto& slice : result) for(auto& cell : slice)
      {
        static const auto unreachable = std::numeric_limits<T>::min();

        cell = unreachable;
      }

      // Seed distance. As per the problem statement, source cells
      // are to be included in distance calculations.
      result[source.y][source.x] = 1;

      return result;
    }();

    const auto withinBounds = [&](const int x, const int y)
    {
      // Top and left bounds.
      if((y < 0) || (x < 0)) return false;

      // Bottom and right bounds.
      if(static_cast<std::size_t>(y) >= this->width ||
         static_cast<std::size_t>(x) >= this->width)
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
      if((this->data[(y * this->width) + x] != ' ') || (distances[y][x] > 0))
      {
        return false;
      }

      return true;
    };

    std::queue<Cell> search;

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
          distances[y][x] = (distances[cell.y][cell.x] + 1);
          search.emplace(x, y);
        }
      }
    }

    std::vector<T> results;

    for(const auto& target : targets)
    {
      results.emplace_back(distances[target.y][target.x]);
    }

    return results;
  }

  private: std::string data;
  private: std::size_t width;
};

template<typename T>
static T getShortestPathLength(const std::vector<std::vector<T>>& edges)
{
  assert(!edges.empty());

  static constexpr auto unreachable = std::numeric_limits<T>::max();

  const auto matrixWidth = edges.size();

  std::vector<T> distances(matrixWidth, unreachable);
  std::vector<bool> used(matrixWidth);

  const auto forEachUnused = [&](const std::function<void(std::size_t)>& fn)
  {
    for(auto i = decltype(matrixWidth){}; i < matrixWidth; ++i)
    {
      if(!used[i]) fn(i);
    }
  };

  // Vertex 0 is the cube's entry-point.
  distances.front() = 0;

  // Dijkstra's algorithm.
  std::for_each(distances.cbegin(), distances.cend(), [&](T)
  {
    const auto i = [&]
    {
      std::pair<T, std::size_t> result(unreachable, 0);

      forEachUnused([&](const std::size_t i)
      {
        if(distances[i] <= result.first) result = {distances[i], i};
      });

      used[result.second] = true;

      return result.second;
    }();

    forEachUnused([&](const std::size_t j)
    {
      if(edges[i][j] && (distances[i] != unreachable))
      {
        const auto distance = (edges[i][j] + distances[i]);

        if(distance < distances[j]) distances[j] = distance;
      }
    });
  });

  // Vertex N is the cube's exit.
  return distances.back();
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  using value_type = char;
  using Maze = Maze<value_type>;
  using Hole = Cell<value_type>;

  std::size_t levelWidth = 0;
  std::string cubeCells;

  std::vector<Maze> levels;
  std::vector<std::vector<Hole>> holes;
  std::vector<std::vector<value_type>> edges;

  const auto getMazeSource = [&](const value_type assignId)
  {
    assert(!cubeCells.empty());

    const auto port =
      cubeCells.find(' ', (cubeCells.size() - (levelWidth * levelWidth)));

    assert(port != std::string::npos);

    return Hole((port % levelWidth), 0, assignId);
  };

  const auto getMazeTarget = [&](const value_type assignId)
  {
    assert(!cubeCells.empty());

    const auto port =
      cubeCells.find(' ', (levelWidth * levelWidth - levelWidth));

    return Hole((port % levelWidth), (levelWidth - 1), assignId);
  };

  while((inputStream >> levelWidth) && inputStream.ignore())
  {
    if(!std::getline(inputStream, cubeCells)) break;

    const auto mazeCellCount = cubeCells.size();
    const auto levelCellCount = (levelWidth * levelWidth);

    // Sanity check.
    assert(mazeCellCount == std::pow(levelWidth, 3));

    holes.clear();
    edges.clear();
    levels.clear();

    value_type holeCount = 0;

    // Fetch cube levels and any floor holes in them.
    for(std::size_t i = 0; i < mazeCellCount; i += levelCellCount)
    {
      holes.emplace_back();

      auto maze = cubeCells.substr(i, levelCellCount);

      for(std::size_t j = 0; j < levelCellCount; ++j)
      {
        if(maze[j] == 'o')
        {
          // We're only interested in obtaining hole coordinates. The maze
          // solver should therefore not see any of the holes in this level's
          // serialized state.
          maze[j] = ' ';

          holes.back().emplace_back((j % levelWidth),
                                     (j / levelWidth),
                                     (++holeCount));
        }
      }

      levels.emplace_back(maze, levelWidth);
    }

    // Inject locations of the cube's entry and exit ports for the
    // bottom and top levels, respectively.
    holes.emplace_back(decltype(holes)::value_type{getMazeSource(0)});
    holes.front().emplace_back(getMazeTarget(++holeCount));

    // Account for the bottom level's hole in the wall.
    assert(holes.size() == (levels.size() + 1));

    const auto isCubeWellFormed = [&]
    {
      for(const auto& mazeHoles : holes)
      {
        if(mazeHoles.empty()) return false;
      }

      return true;
    }();

    // One or more of the cube's levels has no exits.
    if(!isCubeWellFormed)
    {
      std::cout << "0\n";
      continue;
    }

    // Create an adjacency matrix representing the shortest distances
    // between each cell of interest.
    {
      const auto matrixWidth = (holeCount + 1);

      edges.resize(matrixWidth, decltype(edges)::value_type(matrixWidth));

      const auto holesBegin = holes.cbegin(),
                 holesEnd   = std::prev(holes.cend());

      auto level = levels.cbegin();

      for(auto i = holesBegin; i != holesEnd; ++i, ++level)
      {
        const auto bottom = std::next(i);
        const auto bottomWidth = bottom->size();

        for(const auto& top : *i)
        {
          const auto& lengths = level->getPathLengths(top, *bottom);

          for(std::size_t j = 0; j < bottomWidth; ++j)
          {
            edges[top.id][(*bottom)[j].id] = lengths[j];
            edges[(*bottom)[j].id][top.id] = lengths[j];
          }
        }
      }
    }

    std::cout << static_cast<unsigned>(::getShortestPathLength(edges)) << '\n';
  }
}
