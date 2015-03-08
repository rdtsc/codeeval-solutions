#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Descending order.
  auto isSorted = [](const std::string& input)
  {
    static auto greater = std::greater<std::string::value_type>();

    return std::is_sorted(input.cbegin(), input.cend(), greater);
  };

  for(std::string line; std::getline(inputStream, line);)
  {
    // No more permutations left. Inject a zero at the front.
    if(isSorted(line)) line.insert(0, 1, '0');

    std::next_permutation(line.begin(), line.end());

    std::cout << line << '\n';
  }
}
