#include <array>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  using value_type = unsigned char;

  std::array<std::array<value_type, 256>, 256> board = {0};
  std::istringstream tokenizer;

  const auto boardWidth = board[0].size();

  for(std::string line; std::getline(inputStream, line);)
  {
    #define SNIFF(Char) (line.at(5) == (Char))

    enum class Cmd
    {
      Unknown, SetRow, SetCol, QueryRow, QueryCol
    } const command = SNIFF('w') ? Cmd::SetRow   : // SetRo[w]
                      SNIFF('l') ? Cmd::SetCol   : // SetCo[l]
                      SNIFF('R') ? Cmd::QueryRow : // Query[R]ow
                      SNIFF('C') ? Cmd::QueryCol : // Query[C]ol
                                   Cmd::Unknown;

    #undef SNIFF

    #define FOR_EACH_ROW(Index) \
      for(std::size_t (Index) = 0; (Index) < boardWidth; ++(Index))

    #define FOR_EACH_COL(Index) FOR_EACH_ROW(Index)

    switch(command)
    {
      case Cmd::SetRow: case Cmd::SetCol:
      {
        std::size_t index = 0;
        unsigned value = 0;

        tokenizer.clear();
        tokenizer.str(line);

        // Eat the command chunk and get the numeric values that follow.
        tokenizer >> line >> index >> value;

        if(command == Cmd::SetRow) FOR_EACH_ROW(i) board[index][i] = value;

        else FOR_EACH_COL(i) board[i][index] = value;
      }
      break;

      case Cmd::QueryRow: case Cmd::QueryCol:
      {
        std::size_t index = 0;

        tokenizer.clear();
        tokenizer.str(line);

        // Eat the command chunk and get the requested row/column index.
        tokenizer >> line >> index;

        unsigned queryResult = 0;

        if(command == Cmd::QueryRow) FOR_EACH_ROW(i)
        {
          queryResult += board[index][i];
        }

        else FOR_EACH_COL(i) queryResult += board[i][index];

        std::cout << queryResult << '\n';
      }
      break;

      case Cmd::Unknown: assert(false && "Unknown command."); break;
    }

    #undef FOR_EACH_COL
    #undef FOR_EACH_ROW
  }
}
