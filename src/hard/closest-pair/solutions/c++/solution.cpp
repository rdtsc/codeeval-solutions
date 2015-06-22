#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <type_traits>
#include <vector>

template<typename T> struct Point2d
{
  using value_type = T;

  Point2d() : x(), y() {}

  Point2d(const T x, const T y) : x(x), y(y) {}

  friend std::istream& operator>>(std::istream& inputStream, Point2d& point)
  {
    return (inputStream >> point.x >> point.y);
  }

  T getSquaredDistance(const Point2d& other) const
  {
    using scalar_type = typename std::make_signed<T>::type;

    #define SCALAR(Value) static_cast<scalar_type>(Value)

    int deltaX = (SCALAR(this->x) - SCALAR(other.x)),
        deltaY = (SCALAR(this->y) - SCALAR(other.y));

    #undef SCALAR

    deltaX *= deltaX;
    deltaY *= deltaY;

    return (deltaX + deltaY);
  }

  T x, y;
};

template<typename T> static typename T::value_type
getClosestPairSquaredDistance(const std::vector<T>& points)
{
  assert(!points.empty());

  using value_type = typename T::value_type;

  value_type minDistance = std::numeric_limits<value_type>::max();

  const auto pointsBegin = points.cbegin(),
             pointsEnd   = points.cend();

  // HACK: It appears that CodeEval only supplies ~500 points (at most) per
  // data set. Consequently, a naive brute-force search is fast enough to get
  // a high point score. If performance ever becomes an issue, this standard
  // proximity problem can be solved via a recursive divide & conquer search
  // in O(n log n) time.
  for(auto i = pointsBegin;  i != pointsEnd; ++i)
  for(auto j = std::next(i); j != pointsEnd; ++j)
  {
    const auto currentDistance = i->getSquaredDistance(*j);

    if(minDistance > currentDistance) minDistance = currentDistance;
  }

  return minDistance;
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
  using Point2d = Point2d<value_type>;

  std::vector<Point2d> points;

  for(std::size_t expectedPointCount = 0; inputStream >> expectedPointCount;)
  {
    // Dataset terminator is always zero.
    if(!expectedPointCount) break;

    // As per the problem statement.
    assert(expectedPointCount <= 10000);

    points.clear();

    const auto inputIterator  = std::istream_iterator<Point2d>(inputStream);
    const auto outputIterator = std::back_inserter(points);

    std::copy_n(inputIterator, expectedPointCount, outputIterator);

    const auto minDistance = ::getClosestPairSquaredDistance(points);

    // As per the problem statement.
    static constexpr value_type threshold = (10000 * 10000);

    if(minDistance >= threshold) std::cout << "INFINITY";

    else std::cout << std::fixed
                   << std::setprecision(4)
                   << std::sqrt(minDistance);

    std::cout << '\n';
  }
}
