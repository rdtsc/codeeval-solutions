#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class GameOfLife
{
  private: using world_slice_type = std::string;

  public: explicit GameOfLife(std::istream& inputStream) : worldWidth(),
                                                           worldHeight()
  {
    this->world.reserve(100);
    this->worldSnapshot.reserve(100);

    inputStream >> *this;
  }

  public: void tick()
  {
    assert(!this->world.empty());

    for(auto y = decltype(this->worldHeight){}; y < this->worldHeight; ++y)
    for(auto x = decltype(this->worldWidth){}; x < this->worldWidth; ++x)
    {
      const auto neighbors = this->getNeighborCount(x, y);

      // Birth.
      if(neighbors == 3) this->worldSnapshot[y][x] = '*';

      // Death.
      else if(neighbors < 2 || neighbors > 3) this->worldSnapshot[y][x] = '.';
    }

    this->world = this->worldSnapshot;
  }

  private: unsigned getNeighborCount(const std::size_t x,
                                     const std::size_t y) const
  {
    assert(x < this->world.back().size() && y < this->world.size());

    auto isAlive = [&](const int xOffset, const int yOffset)
    {
      // Out of grid bounds.
      if((!x && xOffset < 0) || (x + xOffset >= this->worldWidth) ||
         (!y && yOffset < 0) || (y + yOffset >= this->worldHeight))
      {
        return false;
      }

      return (this->world[y + yOffset][x + xOffset] == '*');
    };

    /*
      Possible neighbor position offsets.

      <A> -1 + + +
      <B>  0 + @ +
      <C>  1 + + +
            -1 0 1
    */

    return isAlive(-1, -1) + isAlive(0, -1) + isAlive(+1, -1) + // <A>
           isAlive(-1,  0) + (000000000000) + isAlive(+1,  0) + // <B>
           isAlive(-1, +1) + isAlive(0, +1) + isAlive(+1, +1);  // <C>
  }

  public: GameOfLife& fastForward(const unsigned generations)
  {
    assert(generations);

    for(auto i = decltype(generations){}; i < generations; ++i)
    {
      this->tick();
    }

    return *this;
  }

  public: void unserialize(const std::string& worldSlice)
  {
    assert(!worldSlice.empty());

    assert(this->world.empty() ||
           this->world.back().size() == worldSlice.size());

    this->world.push_back(worldSlice);
    this->worldSnapshot.push_back(worldSlice);

    this->worldWidth  = worldSlice.size();
    this->worldHeight = this->world.size();
  }

  private: friend std::ostream&
    operator<<(std::ostream& outputStream, const GameOfLife& instance)
  {
    assert(!instance.world.empty());

    for(const auto& slice : instance.world)
    {
      outputStream << slice << '\n';
    }

    return outputStream;
  }

  private: friend std::istream&
    operator>>(std::istream& inputStream, GameOfLife& instance)
  {
    std::string worldSlice;

    worldSlice.reserve(128);

    while(std::getline(inputStream, worldSlice))
    {
      instance.unserialize(worldSlice);
    }

    return inputStream;
  }

  private: std::vector<world_slice_type> world, worldSnapshot;
  private: world_slice_type::size_type worldWidth;
  private: decltype(world)::size_type worldHeight;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Jump to the 10th generation.
  std::cout << GameOfLife(inputStream).fastForward(10) << '\n';
}
