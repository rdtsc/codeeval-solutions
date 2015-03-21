#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::vector<unsigned char> players;

  for(std::string line; std::getline(inputStream, line); players.clear())
  {
    unsigned char values[10] = {};

    // Record player stats.
    for(const auto c : line) if(::isdigit(c))
    {
      const unsigned char digit = c - '0';

      ++values[digit];
      players.push_back(digit);
    }

    // Find the lowest unique ballot.
    const auto winningBallot = std::find(values, std::end(values), 1);

    if(winningBallot == std::end(values)) std::cout << "0\n";

    // Find the player with the correct ballot.
    else
    {
      const auto winningPlayer =
        std::find(players.cbegin(), players.cend(), (winningBallot - values));

      assert(winningPlayer != players.cend());

      std::cout << std::distance(players.cbegin(), winningPlayer) + 1 << '\n';
    }
  }
}
