#include <algorithm>
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

  for(std::string row; std::getline(inputStream, row);)
  {
    auto shiftDistance = std::string::npos;

    if(row.find("XY") != std::string::npos) shiftDistance = 0;

    // Fetch the smallest distance between neighboring X and Y cells on all
    // rows in the input board. This is the maximum number of steps we can
    // take before the X and Y shapes start touching, at least partially.
    else for(std::istringstream tokenizer(row);
             std::getline(tokenizer, row, ',');)
    {
      const std::size_t rowDistance = std::count(row.cbegin(), row.cend(), '.');

      if(rowDistance < shiftDistance) shiftDistance = rowDistance;
    }

    std::cout << shiftDistance << '\n';
  }
}
