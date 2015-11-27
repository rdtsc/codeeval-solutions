#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <sstream>
#include <type_traits>
#include <vector>

template<typename T> struct Point2d
{
  using value_type = T;

  Point2d() : x(), y() {}

  bool inPolygon(const std::vector<Point2d>& outline) const
  {
    const auto pointCount = outline.size();

    unsigned intersections = 0;

    // If a ray cast from our position intersects an odd number of polygon edges
    // then we are located within the polygon. Implemented as described in Q2.3
    // under 2D Polygon Computations in the comp.graphics.algorithms FAQ.
    for(std::size_t i = 0, j = (pointCount - 1); i < pointCount; j = i++)
    {
      // Points on the perimeter are considered to be inside the polygon.
      if(this->isOnSegment(outline[i], outline[j])) return true;

      const float x1 = outline[i].x, y1 = outline[i].y,
                  x2 = outline[j].x, y2 = outline[j].y;

      const auto deltaX = (x2 - x1),
                 deltaY = (y2 - y1);

      if((y1 > this->y) != (y2 > this->y))
      {
        if(((deltaX / deltaY * (this->y - y1)) + x1) > this->x)
        {
          ++intersections;
        }
      }
    }

    return (intersections & 1);
  }

  bool isOnSegment(const Point2d& a, const Point2d& b) const
  {
    const auto& c = *this;

    const typename std::make_signed<T>::type ax = a.x, ay = a.y,
                                             bx = b.x, by = b.y,
                                             cx = c.x, cy = c.y;

    const auto ab = std::hypot((ax - bx), (ay - by)),
               ac = std::hypot((ax - cx), (ay - cy)),
               bc = std::hypot((bx - cx), (by - cy));

    static constexpr auto epsilon =
      std::numeric_limits<decltype(ab)>::epsilon();

    // We're on the segment iff:
    // distance(self, A) + distance(self, B) == distance(A, B).
    return (std::fabs(ac + bc - ab) < epsilon);
  }

  friend std::istream& operator>>(std::istream& inputStream, Point2d& point)
  {
    return (inputStream >> point.x >> point.y);
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

  std::istringstream tokenizer;

  // Add commas and pipes to the list of stream delimiters.
  tokenizer.imbue(std::locale(inputStream.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    categories[','] = categories['|'] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  using Point2d = Point2d<unsigned>;

  std::vector<Point2d> points;

  for(std::string line; std::getline(inputStream, line);)
  {
    points.clear();
    tokenizer.clear();
    tokenizer.str(line);

    using input_iterator =
      std::istream_iterator<decltype(points)::value_type>;

    std::copy(input_iterator(tokenizer), input_iterator(),
      std::back_inserter(points));

    // As per the problem statement, including the test pin's coordinates.
    assert((points.size() > 3) && (points.size() <= 13));

    const auto pin = points.back();

    points.pop_back();

    std::cout << (pin.inPolygon(points) ? "Prisoner" : "Citizen") << '\n';
  }
}
