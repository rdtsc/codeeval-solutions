#include <array>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>

using Board = std::array<std::array<char, 4>, 3>;

static bool boardContainsWord(Board& board,
                              std::string::const_iterator cursor,
                              const std::string::const_iterator wordEnd,
                              const int x,
                              const int y)
{
  static const auto boardWidth  = board[0].size(),
                    boardHeight = board.size();

  // Guard bounds.
  if(x < 0 || y < 0 || (size_t)x >= boardWidth || (size_t)y >= boardHeight)
  {
    return false;
  }

  const auto letter = board[y][x];

  // We've either been here before or there's a mismatch.
  if(letter == ' ' || letter != *cursor) return false;

  // Remove the match.
  board[y][x] = ' ';

  #define TRY_NEXT(xOffset, yOffset) \
    ::boardContainsWord(board, cursor, wordEnd, x + (xOffset), y + (yOffset))

  if(++cursor == wordEnd) return true;

  // Proceed to horizontal and vertical neighbors.
  return TRY_NEXT(-1, 0) || TRY_NEXT(+1, 0) ||
         TRY_NEXT(0, -1) || TRY_NEXT(0, +1);

  #undef TRY_NEXT
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  static struct
  {
    std::size_t count;
    std::size_t values[3];
  } const positions[] =
  {
    /* [0] 1:A */ {2, {0,  8,  0}},
    /* [1] 2:B */ {1, {1,  0,  0}},
    /* [2] 3:C */ {2, {2,  6,  0}},
    /* [3] 4:D */ {1, {9,  0,  0}},
    /* [4] 5:E */ {3, {3, 10, 11}},
    /* [5] 6:F */ {1, {5,  0,  0}},
    /* [6] 7:S */ {2, {4,  7,  0}},
  };

  static const std::size_t letterIndex['Z' - 'A'] =
  {
    1, 2, 3, 4, 5, 6, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 7, 0
  };

  static const Board pristineBoard =
  {
    'A', 'B', 'C', 'E',
    'S', 'F', 'C', 'S',
    'A', 'D', 'E', 'E'
  };

  const auto isPlausible = [&](const std::string& line)
  {
    // Same values as in the master position table.
    std::size_t frequencies[] = {2, 1, 2, 1, 3, 1, 2};

    for(const auto c : line)
    {
      assert(c >= 'A' && c <= 'Z');

      const std::size_t key = (c - 'A');

      // No such letter in the board or too many occurrences of
      // said letter in the input string.
      if(!letterIndex[key] || !frequencies[letterIndex[key] - 1])
      {
        return false;
      }

      --frequencies[letterIndex[key] - 1];
    }

    return true;
  };

  const auto foundWord = [&](const std::string& line)
  {
    const auto& root = positions[letterIndex[line.front() - 'A'] - 1];
    const auto lineBegin = line.cbegin(), lineEnd = line.cend();

    for(std::size_t i = 0; i < root.count; ++i)
    {
      // The search process is destructive.
      Board board = pristineBoard;

      static const auto boardWidth = board[0].size();

      const bool foundWord = ::boardContainsWord(board, lineBegin, lineEnd,
                                                 root.values[i] % boardWidth,
                                                 root.values[i] / boardWidth);

      if(foundWord) return true;
    }

    return false;
  };

  for(std::string line; std::getline(inputStream, line); std::cout << '\n')
  {
    // Bail early if the input doesn't make sense.
    std::cout << (isPlausible(line) && foundWord(line) ? "True" : "False");
  }
}
