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

  // Number of longest lines to return.
  std::size_t sliceSize = 0;

  inputStream >> sliceSize;

  const auto lines = [&]
  {
    std::vector<std::string> result;

    for(std::string line; std::getline(inputStream, line);)
    {
      result.push_back(line);
    }

    // Sort the lines by their lengths in descending order.
    std::sort(result.begin(), result.end(), [](const std::string& lhs,
                                               const std::string& rhs)
    {
      return lhs.size() > rhs.size();
    });

    return result;
  }();

  // Ensure we return at most `lineCount` results.
  if(sliceSize > lines.size()) sliceSize = lines.size();

  for(auto i = lines.cbegin(), j = std::next(i, sliceSize); i != j; ++i)
  {
    std::cout << *i << '\n';
  }
}
