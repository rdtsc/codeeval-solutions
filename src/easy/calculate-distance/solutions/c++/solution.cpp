#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>

template<typename T> struct Point2d
{
  using value_type = T;

  Point2d() : x(), y() {}

  friend std::istream& operator>>(std::istream& inputStream, Point2d& point)
  {
    static constexpr auto ignoreLimit =
      std::numeric_limits<std::streamsize>::max();

    inputStream.ignore(ignoreLimit, '(');
    inputStream >> point.x;

    inputStream.ignore(ignoreLimit, ',');
    inputStream >> point.y;

    return inputStream;
  }

  T x, y;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(Point2d<int> a, b; inputStream >> a >> b;)
  {
    // Distance formula.
    const unsigned distance = ::sqrt(::pow(b.x - a.x, 2) +
                                     ::pow(b.y - a.y, 2));

    std::cout << distance << '\n';
  }
}
