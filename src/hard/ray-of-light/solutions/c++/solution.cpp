#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

struct Entity
{
  using value_type = char;

  Entity() = delete;

  static bool isTransparent(const value_type entity)
  {
    switch(entity)
    {
      case Entity::space:
      case Entity::prism:
      case Entity::rayMinor:
      case Entity::rayMajor:
      case Entity::rayJunction:
      {
        return true;
      }
    }

    return false;
  }

  static const value_type space       = ' ',  // Ether.
                          prism       = '*',  // Diffractive.
                          tower       = 'o',  // Absorptive.
                          mirror      = '#',  // Reflective.
                          rayMajor    = '\\', // 135° and 315° ray segment.
                          rayMinor    = '/',  // 45° and 225° ray segment.
                          rayJunction = 'X';  // Ray segment intersection.
};

enum class Trajectory : unsigned short
{
  #define DEFINE(Value) A##Value = (Value)
  #define VALUE_TYPE std::underlying_type<Trajectory>::type

  DEFINE( 45), DEFINE(135),
  DEFINE(225), DEFINE(315),

  Undefined = std::numeric_limits<VALUE_TYPE>::max()

  #undef VALUE_TYPE
  #undef DEFINE
};

static constexpr Trajectory operator"" _deg(const unsigned long long angle)
{
  #define IS(Value) angle == (Value) ? (Trajectory::A##Value)

  return IS( 45) : IS(135) :
         IS(225) : IS(315) : throw std::domain_error("Undefined");

  #undef IS
}

struct EnumHasher
{
  template<typename T> std::size_t operator()(const T value) const
  {
    return static_cast<std::size_t>(value);
  }
};

struct Photon
{
  Photon() : Photon(0, 0, Trajectory::Undefined) {}

  Photon(const int x, const int y, const Trajectory trajectory) :
    x(x), y(y), age(), trajectory(trajectory) {}

  void move(const int xOffset, const int yOffset)
  {
    this->x += xOffset;
    this->y += yOffset;
  }

  void reflect(const int xOffset,
               const int yOffset,
               const Trajectory trajectory)
  {
    this->move(xOffset, yOffset);
    this->trajectory = trajectory;
  }

  Photon clone(const Trajectory trajectory) const
  {
    auto clone = *this;

    clone.trajectory = trajectory;

    return clone;
  }

  bool operator==(const Photon& other) const
  {
    return (this->x == other.x) &&
           (this->y == other.y) &&
           (this->trajectory == other.trajectory);
  }

  int x, y;
  unsigned age;
  Trajectory trajectory;
};

struct PhotonTraits
{
  Entity::value_type symbol;
  int directionalOffsets[2];
  Trajectory complements[2];
  std::function<void(Photon&)> reflect;
};

template<unsigned PhotonLifeSpan> class World
{
  public: World& render()
  {
    for(Photon lastFrame; !this->photons.empty();)
    {
      this->tick();

      if(!this->photons.empty())
      {
        const auto& photon = this->photons.front();

        // No more visible updates left for this packet.
        if(photon == lastFrame)
        {
          this->photons.pop();

          continue;
        }

        lastFrame = photon;
      }
    }

    return *this;
  }

  private: void tick()
  {
    assert(!this->photons.empty());

    const auto frameWidth = this->cells.size();

    auto& photon = this->photons.front();

    if(++photon.age > PhotonLifeSpan)
    {
      return this->photons.pop();
    }

    const auto exists = [&](const int xOffset, const int yOffset)
    {
      const auto x = (photon.x + xOffset),
                 y = (photon.y + yOffset),
                 w = static_cast<int>(frameWidth);

      return ((x >= 0) && (y >= 0) && (x < w) && (y < w));
    };

    const auto at = [&](const int xOffset, const int yOffset) -> char&
    {
      assert(exists(xOffset, yOffset));

      return this->cells[photon.y + yOffset][photon.x + xOffset];
    };

    const auto reflect = [&](Photon& self,
                             const Trajectory trajectory,
                             const int xOffset,
                             const int yOffset)
    {
      const auto neighbor =
        this->cells[self.y + yOffset][self.x + xOffset];

      if(Entity::isTransparent(neighbor))
      {
        self.reflect(xOffset, yOffset, trajectory);
      }
    };

    /*
                        ┌─/───┐ ┌───\─┐ ┌─────┐ ┌─────┐    -1,-1   +1,-1
          135°│ 45°     │/    │ │    \│ │ 045 │ │ 135 │    ┌───┬───┬───┐
            \ │ /       │ 225 │ │ 315 │ │    /│ │\    │    │ \ │ ░ │ / │
         II  \│/   I    └─────┘ └─────┘ └───/─┘ └─\───┘    ├───┼───┼───┤
         ─────X─────                                       │ ░ │ X │ ░ │
         III /│\  IV    ┌─────┐ ┌─────┐ ┌─────┐ ┌─────┐    ├───┼───┼───┤
            / │ \       │ 225 / \ 315 │ │/    │ │    \│    │ / │ ░ │ \ │
          225°│ 315°    │    /│ │\    │ / 045 │ │ 135 \    └───┴───┴───┘
                        └─────┘ └─────┘ └─────┘ └─────┘    -1,+1   +1,+1
    */

    using Traits = PhotonTraits;

    static const std::unordered_map<Trajectory, Traits, EnumHasher> rules =
    {
      // Quadrant I.
      {
        45_deg,
        {
          Entity::rayMinor, {+1, -1}, {135_deg, 315_deg}, [&](Photon& self)
          {
            // Top edge.
            if(self.y == 1) reflect(self, 315_deg, +1, +0);

            // Right edge.
            else reflect(self, 135_deg, +0, -1);
          }
        }
      },

      // Quadrant II.
      {
        135_deg,
        {
          Entity::rayMajor, {-1, -1}, {45_deg, 225_deg}, [&](Photon& self)
          {
            // Top edge.
            if(self.y == 1) reflect(self, 225_deg, -1, +0);

            // Left edge.
            else reflect(self, 45_deg, +0, -1);
          }
        }
      },

      // Quadrant III.
      {
        225_deg,
        {
          Entity::rayMinor, {-1, +1}, {135_deg, 315_deg}, [&](Photon& self)
          {
            // Bottom edge.
            if(self.y == (frameWidth - 2)) reflect(self, 135_deg, -1, +0);

            // Left edge.
            else reflect(self, 315_deg, +0, +1);
          }
        }
      },

      // Quadrant IV.
      {
        315_deg,
        {
          Entity::rayMajor, {+1, +1}, {45_deg, 225_deg}, [&](Photon& self)
          {
            // Bottom edge.
            if(self.y == (frameWidth - 2)) reflect(self, 45_deg, +1, +0);

            // Right edge.
            else reflect(self, 225_deg, +0, +1);
          }
        }
      }
    };

    assert(rules.find(photon.trajectory) != rules.cend());

    const auto& traits = rules.at(photon.trajectory);

    // Render self.
    {
      auto& origin = at(0, 0);

      if(origin == Entity::space) origin = traits.symbol;

      else if((origin == Entity::rayMinor) || (origin == Entity::rayMajor))
      {
        if(origin != traits.symbol) origin = Entity::rayJunction;
      }
    }

    const auto x = traits.directionalOffsets[0],
               y = traits.directionalOffsets[1];

    // Decay self if we've departed from the world's bounds.
    if(!exists(x, y))
    {
      photon.age = PhotonLifeSpan;

      return;
    }

    switch(at(x, y))
    {
      // Propel self.
      case Entity::space:
      case Entity::rayMinor:
      case Entity::rayMajor:
      case Entity::rayJunction:
      {
        photon.move(x, y);
      }
      break;

      // Split self.
      case Entity::prism:
      {
        // Propagation inside prisms is free in terms of aging.
        --photon.age;

        photon.move(x, y);
        this->photons.emplace(photon.clone(traits.complements[0]));
        this->photons.emplace(photon.clone(traits.complements[1]));
      }
      break;

      // Reflect self.
      case Entity::mirror: traits.reflect(photon); break;
    }
  }

  private: friend std::istream& operator>>(std::istream& inputStream,
                                           World& self)
  {
    std::string payload;

    if(!std::getline(inputStream, payload)) return inputStream;

    const std::size_t worldWidth = std::sqrt(payload.size());

    assert(worldWidth >= 3);
    assert((worldWidth * worldWidth) == payload.size());

    auto& cells = self.cells;

    cells.clear();

    for(std::size_t i = 0; i < worldWidth; ++i)
    {
      cells.emplace_back(payload.substr((i * worldWidth), worldWidth));
    }

    // The world is supposed to be square.
    assert(!cells.empty() && (cells.size() == worldWidth));

    // Slices should be of equal width and contain only whitelisted entities.
    assert(std::all_of(cells.cbegin(), cells.cend(), [&](const std::string& row)
    {
      const auto isAligned = (row.size() == cells.front().size());

      const auto isWellFormed =
        (row.find_first_not_of("/\\ o*#") == std::string::npos);

      return (isAligned && isWellFormed);
    }));

    // Only one injection port should be present.
    assert(std::count_if(payload.cbegin(), payload.cend(), [](const char c)
    {
      return (c == '\\') || (c == '/');
    }) == 1);

    // Inject the initial photon packet.
    {
      const auto injectionPort = payload.find_first_of("\\/");

      const auto x = (injectionPort % worldWidth),
                 y = (injectionPort / worldWidth);

      const auto packetTrajectory = [&]
      {
        const bool isForwardSlash = (payload[injectionPort] == '/');

        if(!y) return (isForwardSlash ? 225_deg : 315_deg);
        if(!x) return (isForwardSlash ?  45_deg : 315_deg);

        if(y == (worldWidth - 1)) return (isForwardSlash ?  45_deg : 135_deg);
        if(x == (worldWidth - 1)) return (isForwardSlash ? 225_deg : 135_deg);

        assert(false);

        return Trajectory::Undefined;
      }();

      cells[y][x] = Entity::space;

      self.photons.emplace(x, y, packetTrajectory);
    }

    return inputStream;
  }

  private: friend std::ostream& operator<<(std::ostream& outputStream,
                                           const World& self)
  {
    for(const auto& slice : self.cells) std::cout << slice;

    return outputStream;
  }

  private: std::vector<std::string> cells;
  private: std::queue<Photon> photons;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(World<20> world; inputStream >> world;)
  {
    std::cout << world.render() << '\n';
  }
}
