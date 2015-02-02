#include <cassert>
#include <fstream>
#include <iostream>

struct BoardPosition
{
  friend std::istream& operator>>(std::istream& inputStream, BoardPosition& pos)
  {
    return inputStream >> pos.x >> pos.y;
  }

  char x;
  unsigned y;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(BoardPosition knightPosition; inputStream >> knightPosition;)
  {
    const auto c = knightPosition.x;
    const auto n = knightPosition.y;

    assert(c >= 'a' && c <= 'h');
    assert(n >= 1 && n <= 8);

    /*
      Possible knight position offsets.
      Could just use a LUT for all 64 starting positions, instead.

      5   @   @
      4 @       @
      3     N
      2 @       @
      1   @   @
        A B C D E
    */

    auto dump = [c, n](const int xOffset, const int yOffset)
    {
      std::cout << static_cast<decltype(c)>(c + xOffset) <<
                   static_cast<decltype(n)>(n + yOffset) << ' ';
    };

    if(c >= 'c')
    {
      if(n >= 2) dump(-2, -1); // A2
      if(n <= 7) dump(-2, +1); // A4
    }

    if(c >= 'b')
    {
      if(n >= 3) dump(-1, -2); // B1
      if(n <= 6) dump(-1, +2); // B5
    }

    if(c <= 'g')
    {
      if(n >= 3) dump(+1, -2); // D1
      if(n <= 6) dump(+1, +2); // D5
    }

    if(c <= 'f')
    {
      if(n >= 2) dump(+2, -1); // E2
      if(n <= 7) dump(+2, +1); // E4
    }

    std::cout << '\n';
  }
}
