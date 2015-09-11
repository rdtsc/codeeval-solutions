#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

class World
{
  public: unsigned getLakeCount()
  {
    assert(!this->slices.empty());

    const std::size_t worldWidth  = this->slices.front().size(),
                      worldHeight = this->slices.size();

    // Clear history.
    {
      this->breadcrumbs.clear();

      using breadcrumb_row = decltype(this->breadcrumbs)::value_type;

      this->breadcrumbs.resize(worldHeight, breadcrumb_row(worldWidth));
    }

    // Lake fragments are denoted by lowercase o's.
    const auto isLake = [&](const int x, const int y)
    {
      return (!this->breadcrumbs[y][x] && (this->slices[y][x] == 'o'));
    };

    const std::function<void(int, int)> visit = [&](const int x, const int y)
    {
      this->breadcrumbs[y][x] = true;

      static struct {int x, y;} const offsets[] =
      {
        /*
          Possible neighbor position offsets.

          <A> -1 + + +
          <B>  0 + @ +
          <C>  1 + + +
                -1 0 1
        */

        {-1, -1}, {0, -1}, {1, -1}, // <A>
        {-1,  0}, /* @ */  {1,  0}, // <B>
        {-1,  1}, {0,  1}, {1,  1}  // <C>
      };

      // Guard world bounds.
      const auto check = [&](const int x, const int y)
      {
        if((x < 0) || ((std::size_t)x >= worldWidth) ||
           (y < 0) || ((std::size_t)y >= worldHeight))
        {
          return false;
        }

        return isLake(x, y);
      };

      // Visit neighbors.
      for(const auto& offset : offsets)
      {
        const auto newX = (x + offset.x),
                   newY = (y + offset.y);

        if(check(newX, newY)) visit(newX, newY);
      }
    };

    return [&]
    {
      unsigned lakeCount = 0;

      for(std::size_t y = 0; y < worldHeight; ++y)
      for(std::size_t x = 0; x < worldWidth;  ++x)
      {
        if(isLake(x, y))
        {
          ++lakeCount;
          visit(x, y);
        }
      }

      return lakeCount;
    }();
  }

  private: friend std::istream& operator>>(std::istream& inputStream,
                                           World& world)
  {
    auto& slices = world.slices;
    auto& data = world.sliceBuffer;

    slices.clear();
    data.clear();

    if(!std::getline(inputStream, data)) return inputStream;

    // Smush spaces.
    data.erase(std::remove(data.begin(), data.end(), ' '), data.end());

    const auto dataEnd = data.cend();

    // Explode slices.
    for(auto head = data.cbegin(), tail = head;; head = tail)
    {
      while(tail != dataEnd && *tail != '|') ++tail;

      slices.emplace_back(head, tail);

      // The world should only contain lakes ('o') and forests ('#').
      assert(std::all_of(slices.back().cbegin(), slices.back().cend(),
        [](const char c)
      {
        return (c == 'o' || c == '#');
      }));

      if(tail++ == dataEnd) break;
    }

    // As per the problem statement.
    assert((slices.front().size() >= 03) && (slices.size() >= 03));
    assert((slices.front().size() <= 30) && (slices.size() <= 30));

    // Confirm world rectangularity.
    assert(std::all_of(slices.cbegin(), slices.cend(),
      [&](const std::string& slice)
    {
      return (slice.size() == slices.front().size());
    }));

    return inputStream;
  }

  private: std::vector<std::string> slices;
  private: std::vector<std::vector<bool>> breadcrumbs;
  private: std::string sliceBuffer;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(World world; inputStream >> world;)
  {
    std::cout << world.getLakeCount() << '\n';
  }
}
