#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
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

  for(std::string word, mask; inputStream >> word >> mask;)
  {
    // Guaranteed by the problem statement.
    assert(word.size() == mask.size());

    auto digit = mask.cbegin();

    std::transform(word.begin(), word.end(), word.begin(), [&](const char c)
    {
      assert(digit != mask.cend());

      return (*digit++ == '1' ? ::toupper(c) : c);
    });

    std::cout << word << '\n';
  }
}
