#include <cassert>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iomanip>
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

  std::cout << std::setprecision(2) << std::fixed;

  for(std::string line; std::getline(inputStream, line);)
  {
    std::size_t count[2] = {};
    enum {LOWERCASE, UPPERCASE};

    // Input lines are guaranteed to consist of only letters.
    for(const auto c : line) ++count[!::islower(c)];

    const auto multiplier = line.size() * 0.01f;

    std::cout << "lowercase: " << (count[LOWERCASE] / multiplier) << ' '
              << "uppercase: " << (count[UPPERCASE] / multiplier) << '\n';
  }
}
