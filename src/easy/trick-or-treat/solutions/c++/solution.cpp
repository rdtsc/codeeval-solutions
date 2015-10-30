#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>

template<typename T>
static bool extractFrom(std::istream& inputStream,
                        const char delimiter,
                        T& value)
{
  static constexpr auto ignoreLimit =
    std::numeric_limits<std::streamsize>::max();

  return inputStream.ignore(ignoreLimit, delimiter) >> value;
}

template<typename T, typename... Values>
static bool extractFrom(std::istream& inputStream,
                        const char delimiter,
                        T& value,
                        Values&... values)
{
  return ::extractFrom(inputStream, delimiter, value) &&
         ::extractFrom(inputStream, delimiter, values...);
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  unsigned v = 0, z = 0, w = 0, h = 0;

  while(::extractFrom(inputStream, ':', v, z, w, h))
  {
    const auto take = v * 3 + // Vampires.
                      z * 4 + // Zombies.
                      w * 5;  // Witches.

    const auto loot   = (take * h),
               shares = (v + z + w);

    std::cout << (loot / shares) << '\n';
  }
}
