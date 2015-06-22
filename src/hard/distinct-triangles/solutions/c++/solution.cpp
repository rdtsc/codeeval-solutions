#include <array>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <locale>
#include <utility>
#include <vector>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Add commas and semicolons to the list of stream delimiters.
  inputStream.imbue(std::locale(inputStream.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    categories[','] = categories[';'] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  using matrix_value_type = unsigned char;
  using matrix_type = std::array<std::array<matrix_value_type, 100>, 100>;

  for(std::size_t vertexCount = 0, edgeCount = 0;
      inputStream >> vertexCount >> edgeCount;)
  {
    assert(vertexCount >= 001 && edgeCount >= 001);
    assert(vertexCount <= 100 && edgeCount <= 100);

    matrix_type edges = {0};

    for(std::size_t i = 0; i < edgeCount; ++i)
    {
      std::pair<unsigned, unsigned> edge;

      if(!(inputStream >> edge.first >> edge.second)) assert(false);

      assert((edge.first < vertexCount) && (edge.second < vertexCount));

      edges[edge.first][edge.second] = 1;
      edges[edge.second][edge.first] = 1;
    }

    const auto cubedAdjacencyTrace = [&]
    {
      std::pair<matrix_type, matrix_type> scratch;

      scratch.first = edges;

      // Cube the adjacency matrix.
      for(std::size_t i = 0; i < 2; ++i)
      {
        for(std::size_t y = 0; y < vertexCount; ++y)
        for(std::size_t x = 0; x < vertexCount; ++x)
        {
          auto cell = 0;

          for(std::size_t j = 0; j < vertexCount; ++j)
          {
            cell += scratch.first[y][j] * edges[j][x];
          }

          scratch.second[y][x] = cell;
        }

        scratch.first = scratch.second;
      }

      std::size_t result = 0;

      // Trace the cubed adjacency matrix.
      for(std::size_t i = 0; i < vertexCount; ++i)
      {
        result += scratch.first[i][i];
      }

      return result;
    }();

    // Looking for paths of L <- 3; ignoring duplicates.
    std::cout << (cubedAdjacencyTrace / 6) << '\n';
  }
}
