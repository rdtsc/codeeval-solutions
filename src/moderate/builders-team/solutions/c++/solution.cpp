#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <locale>
#include <sstream>
#include <utility>
#include <vector>

template<typename T, std::size_t Width>
static bool isCircuit(const std::initializer_list<T>& indices,
                      const std::size_t offset,
                      const bool(& connections)[Width][Width])
{
  assert(indices.size() && !(indices.size() & 1));

  const auto indicesBegin = indices.begin(),
             indicesEnd   = std::prev(indices.end());

  // Attempt to walk the circuit.
  for(auto i = indicesBegin; i != indicesEnd; ++i)
  {
    const auto x = (*(i + 0) + offset - 1),
               y = (*(i + 1) + offset - 1);

    assert((x >= 0) && (x < Width));
    assert((y >= 0) && (y < Width));

    if(!connections[x][y]) return false;
  }

  // The last vertex should link up with the head to complete the circuit.
  return connections[*indicesBegin + offset - 1][*indicesEnd + offset - 1];
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::istringstream tokenizer;

  // Add pipes to the list of stream delimiters.
  tokenizer.imbue(std::locale(tokenizer.getloc(), []
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

  static constexpr std::size_t mapWidth = 5,
                               mapArea  = (mapWidth * mapWidth);

  for(std::string line; std::getline(inputStream, line);)
  {
    tokenizer.clear();
    tokenizer.str(line);

    bool connections[mapArea][mapArea] = {};

    // Unserialize connections.
    for(unsigned lhs = 0, rhs = 0; tokenizer >> lhs >> rhs;)
    {
      assert(lhs != rhs);
      assert((lhs >= 1) && (lhs <= mapArea));
      assert((rhs >= 1) && (rhs <= mapArea));

      // Zero-based indices.
      --lhs; --rhs;

      connections[lhs][rhs] = true;
      connections[rhs][lhs] = true;
    }

    const auto squares = [&]
    {
      unsigned result = 0;

      // Unit squares based on the diagram in the problem statement.
      static constexpr std::initializer_list<std::size_t> units[] =
      {
        /* 1x1 */ {1, 2, 7, 6},
        /* 2x2 */ {1, 2, 3, 8, 13, 12, 11, 6},
        /* 3x3 */ {1, 2, 3, 4, 9, 14, 19, 18, 17, 16, 11, 6},
        /* 4x4 */ {1, 2, 3, 4, 5, 10, 15, 20, 25, 24, 23, 22, 21, 16, 11, 6}
      };

      #define IS_CIRCUIT(Unit, Pad) ::isCircuit((Unit), (Pad), connections)

      // 1x1.
      for(std::size_t x = 0; x < (mapWidth - 1); ++x)
      for(std::size_t y = 0; y < (mapWidth * 4); y += mapWidth)
      {
        if(IS_CIRCUIT(units[0], (x + y))) ++result;
      }

      // 2x2.
      for(std::size_t x = 0; x < (mapWidth - 2); ++x)
      for(std::size_t y = 0; y < (mapWidth * 3); y += mapWidth)
      {
        if(IS_CIRCUIT(units[1], (x + y))) ++result;
      }

      // 3x3.
      for(std::size_t x = 0; x < (mapWidth - 3); ++x)
      for(std::size_t y = 0; y < (mapWidth * 2); y += mapWidth)
      {
        if(IS_CIRCUIT(units[2], (x + y))) ++result;
      }

      // 4x4.
      if(IS_CIRCUIT(units[3], 0)) ++result;

      #undef IS_CIRCUIT

      return result;
    }();

    std::cout << squares << '\n';
  }
}
