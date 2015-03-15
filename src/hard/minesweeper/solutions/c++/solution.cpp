#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

struct Minefield
{
  std::vector<std::string> data;
};

static std::istream& operator>>(std::istream& inputStream, Minefield& minefield)
{
  minefield.data.clear();

  // Minefield: M rows by N columns.
  std::size_t m = 0, n = 0;
  std::string data;

  static constexpr auto ignoreLimit =
    std::numeric_limits<std::streamsize>::max();

  inputStream >> m;
  inputStream.ignore(ignoreLimit, ',');
  inputStream >> n;
  inputStream.ignore(ignoreLimit, ';');
  inputStream >> data;

  if(!inputStream) return inputStream;

  assert(m && n && (data.size() == m * n));

  // Reconstruct the minefield. Asterisks represent mines.
  {
    minefield.data.resize(m, std::string(n, '.'));

    auto cell = data.cbegin();

    for(std::size_t y = 0; y < m; ++y)
    for(std::size_t x = 0; x < n; ++x)
    {
      assert(*cell == '.' || *cell == '*');

      minefield.data[y][x] = *cell++;
    }
  }

  for(std::size_t y = 0; y < m; ++y)
  for(std::size_t x = 0; x < n; ++x)
  {
    // Mines get dumped verbatim. Empty cells display the total number of
    // mines in all eight of their adjacent fields.
    if(minefield.data[y][x] == '*') continue;

    auto isMine = [&](const int xOffset, const int yOffset)
    {
      // Out of grid bounds.
      if((!x && xOffset < 0) || (x + xOffset >= n) ||
         (!y && yOffset < 0) || (y + yOffset >= m))
      {
        return false;
      }

      return (minefield.data[y + yOffset][x + xOffset] == '*');
    };

    /*
      Possible neighbor position offsets.

      <A> -1 + + +
      <B>  0 + @ +
      <C>  1 + + +
            -1 0 1
    */

    const unsigned adjacentMineCount =
      isMine(-1, -1) + isMine(0, -1) + isMine(+1, -1) + // <A>
      isMine(-1,  0) + (00000000000) + isMine(+1,  0) + // <B>
      isMine(-1, +1) + isMine(0, +1) + isMine(+1, +1);  // <C>

    minefield.data[y][x] = adjacentMineCount + '0';
  }

  return inputStream;
}

static std::ostream& operator<<(std::ostream& outputStream,
                                const Minefield& minefield)
{
  for(const auto& row : minefield.data) outputStream << row;

  return outputStream;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(Minefield minefield; inputStream >> minefield;)
  {
    std::cout << minefield << '\n';
  }
}
