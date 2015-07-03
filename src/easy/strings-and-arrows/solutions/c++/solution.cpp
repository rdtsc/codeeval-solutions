#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

static unsigned getSubstringCount(const std::string& source,
                                  const std::string& target)
{
  unsigned result = 0;

  static constexpr auto notFound = std::string::npos;

  for(auto cursor = source.find(target); cursor != notFound; ++result)
  {
    cursor = source.find(target, ++cursor);
  }

  return result;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(std::string line; std::getline(inputStream, line);)
  {
    const auto arrowCount = ::getSubstringCount(line, "<--<<") +
                            ::getSubstringCount(line, ">>-->");

    std::cout << arrowCount << '\n';
  }
}
