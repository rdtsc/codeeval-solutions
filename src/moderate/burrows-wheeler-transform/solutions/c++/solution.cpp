#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
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

  std::vector<std::size_t> mapping;

  for(std::string line, lhs, rhs; std::getline(inputStream, line);)
  {
    assert(std::count_if(line.begin(), line.end(), [](const char c)
    {
      return (c == '$' || c == '|');
    }) == 2);

    // Kill the trailing pipe.
    line.pop_back();

    /*
      Encoded: "yyeep$-aass"; Last chars of all sorted source string rotations.
      Decoded: "easy-peasy$"; The '$' char always terminates the source string.

        #  RHS  =>  LHS (sort)  =>  Map LHS to RHS  =>  Index RHS via Mapping
        -  ---      ----------      --------------      ---------------------
        0   y           $                  5            RHS[ LHS[5] = 3 ] : e
        1   y           -                  6            RHS[ LHS[3] = 8 ] : a
        2   e           a                  7            RHS[ LHS[8] = A ] : s
        3   e           a                  8            RHS[ LHS[A] = 1 ] : y
        4   p           e                  2            RHS[ LHS[1] = 6 ] : -
        5   $           e                  3            RHS[ LHS[6] = 4 ] : p
        6   -           p                  4            RHS[ LHS[4] = 2 ] : e
        7   a           s                  9            RHS[ LHS[2] = 7 ] : a
        8   a           s                 10            RHS[ LHS[7] = 9 ] : s
        9   s           y                  0            RHS[ LHS[9] = 0 ] : y
        A   s           y                  1            RHS[ LHS[0] = 5 ] : $
    */

    lhs = line; rhs = line;
    std::sort(lhs.begin(), lhs.end());

    mapping.clear();
    mapping.reserve(line.size());

    using value_type = decltype(mapping)::value_type;

    value_type mappingIndex = 0;

    for(const auto c : lhs)
    {
      const auto row = rhs.find(c);

      if(c == '$') mappingIndex = row;

      mapping.push_back(row);
      rhs[row] = '\0';
    }

    rhs.clear();

    std::for_each(mapping.cbegin(), mapping.cend(), [&](const value_type)
    {
      mappingIndex = mapping[mappingIndex];
      rhs += line[mappingIndex];
    });

    std::cout << rhs << '\n';
  }
}
