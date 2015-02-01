#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(std::size_t n = 0; inputStream >> n;)
  {
    using row_type = std::vector<std::uint_fast32_t>;

    std::vector<row_type> matrix;

    // Fetch matrix terms.
    for(std::size_t i = 0; i <= n; ++i)
    {
      const auto row = [&]
      {
        row_type result;
        std::string line;

        std::getline(inputStream, line);

        if(line.empty()) return result;

        std::istringstream tokenizer(line);

        for(row_type::value_type term = 0; tokenizer >> term;)
        {
          result.push_back(term);

          if(tokenizer.peek() == ',') tokenizer.ignore();
        }

        return result;
      }();

      if(!row.empty()) matrix.push_back(row);
    }

    // Patch the bottom row and rightmost column with local sums moving
    // from the destination at the bottom right.
    for(std::size_t y = --n, x = y; y--;)
    {
      matrix[y][x] += matrix[y + 1][x];
      matrix[x][y] += matrix[x][y + 1];
    }

    // Start from the destination at the bottom right and work our way up to the
    // top left by selecting and recording the smallest local sum at each hop.
    for(std::size_t y = n; y--;) for(std::size_t x = n; x--;)
    {
      matrix[y][x] += matrix[y + 1][x] < matrix[y][x + 1] ? matrix[y + 1][x] :
                                                            matrix[y][x + 1];
    }

    // We're at the origin.
    std::cout << matrix[0][0] << '\n';
  }
}
