#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

template<typename T> struct Point2d
{
  Point2d() : x(), y() {}

  T x, y;
};

template<typename T> static std::istream& operator>>(std::istream& inputStream,
                                                     Point2d<T>& point)
{
  return inputStream >> point.x >> point.y;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::string bearing;

  for(Point2d<int> pos, dst; inputStream >> pos >> dst; bearing.clear())
  {
    if(pos.y != dst.y)  bearing.push_back(pos.y > dst.y ? 'S' : 'N');
    if(pos.x != dst.x)  bearing.push_back(pos.x > dst.x ? 'W' : 'E');

    // If nothing's written to the buffer, we're already at the destination.
    if(bearing.empty()) bearing = "here";

    std::cout << bearing << '\n';
  }
}
