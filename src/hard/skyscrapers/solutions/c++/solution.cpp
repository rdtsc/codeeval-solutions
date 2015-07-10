#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <vector>

template<typename T> using Skyline = std::map<T, T>;

template<typename T> struct Building
{
  using value_type = T;

  Building() : x(), height() {}

  friend std::istream& operator>>(std::istream& inputStream, Building& building)
  {
    const auto jumpTo = [&](const char c) -> decltype(inputStream)
    {
      assert(::isprint(c));

      static constexpr auto ignoreLimit =
        std::numeric_limits<std::streamsize>::max();

      return inputStream.ignore(ignoreLimit, c);
    };

    T left = 0, right = 0, height = 0;

    jumpTo('(') >> left;
    jumpTo(',') >> height;
    jumpTo(',') >> right;
    jumpTo(')');

    if(!inputStream) return inputStream;

    // As per the problem statement.
    assert(height >= 1 && height <= 100);
    assert(left <= 1e4 && right <= 1e4);

    building.x[0]   = left;
    building.x[1]   = right;
    building.height = height;

    return inputStream;
  }

  T x[2], height;
};

template<typename T>
static std::ostream& operator<<(std::ostream& outputStream,
                                const Skyline<T>& skyline)
{
  T trace = 0;

  for(const auto& point : skyline)
  {
    const bool isVertex = (point.second != trace);

    if(isVertex)
    {
      outputStream << point.first  << ' ' << point.second << ' ';

      trace = point.second;
    }
  }

  return outputStream;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  using value_type = unsigned;

  Skyline<value_type> skyline;

  for(Building<value_type> building; inputStream >> building;)
  {
    // Trace the current building.
    for(value_type i = building.x[0]; i < building.x[1]; ++i)
    {
      skyline[i] = std::max(skyline[i], building.height);
    }

    // Right face. Either overlapping or terminal.
    skyline[building.x[1]] = std::max(skyline[building.x[1]], value_type());

    switch(inputStream.peek()) case '\n': case EOF:
    {
      std::cout << skyline << '\n';

      skyline.clear();
    }
  }
}
