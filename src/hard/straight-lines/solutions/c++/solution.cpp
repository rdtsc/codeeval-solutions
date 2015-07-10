#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <locale>
#include <vector>

template<typename T> struct Point2d
{
  using value_type = T;

  Point2d() : x(), y() {}

  friend std::istream& operator>>(std::istream& inputStream, Point2d& point)
  {
    return (inputStream >> point.x >> point.y);
  }

  bool operator==(const Point2d& other) const
  {
    return (this->x == other.x) &&
           (this->y == other.y);
  }

  T x, y;
};

template<typename T> struct Segment2d
{
  using value_type = T;

  Segment2d(const Point2d<T>& a,
            const Point2d<T>& b,
            const Point2d<T>& c) : a(a), b(b), c(c) {}

  Point2d<T> a, b, c;
};

template<typename T> static inline bool isCollinear(const Point2d<T>& a,
                                                    const Point2d<T>& b,
                                                    const Point2d<T>& c)
{
  #define COMPONENT(P1, P2, P3) ((P1).x * ((P2).y - (P3).y))

  const auto area = COMPONENT(a, b, c) +
                    COMPONENT(b, c, a) +
                    COMPONENT(c, a, b);

  return (area == 0);

  #undef COMPONENT
}

template<typename T>
static unsigned getLineCount(const std::vector<Point2d<T>>& points)
{
  const auto pointsBegin = points.cbegin(),
             pointsEnd   = points.cend();

  std::vector<Segment2d<T>> segments;

  auto segmentsBegin = segments.cbegin(),
       segmentsEnd   = segments.cend();

  for(auto i = pointsBegin;  i != pointsEnd; ++i)
  for(auto j = std::next(i); j != pointsEnd; ++j)
  for(auto k = std::next(j); k != pointsEnd; ++k)
  {
    if(::isCollinear(*i, *j, *k))
    {
      const auto isDupe = [&](const Segment2d<T>& segment)
      {
        const auto lhs = (j->y - i->y) * (segment.b.x - segment.a.x),
                   rhs = (j->x - i->x) * (segment.b.y - segment.a.y);

        const bool isSameSlope = (lhs == rhs);

        if(!isSameSlope) return false;

        #define IS_POINT_SHARED(Point) \
          (segment.a == (Point) || segment.b == (Point) || segment.c == (Point))

        const bool hasSharedPoints = IS_POINT_SHARED(*i) ||
                                     IS_POINT_SHARED(*j) ||
                                     IS_POINT_SHARED(*k);

        return hasSharedPoints;

        #undef IS_POINT_SHARED
      };

      if(std::find_if(segmentsBegin, segmentsEnd, isDupe) == segmentsEnd)
      {
        segments.emplace_back(*i, *j, *k);

        segmentsBegin = segments.cbegin();
        segmentsEnd   = segments.cend();
      }
    }
  }

  return segments.size();
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Add pipes to the list of stream delimiters.
  inputStream.imbue(std::locale(inputStream.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    categories['|'] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  using Point2d = Point2d<int>;

  std::vector<Point2d> points;

  for(Point2d point; inputStream >> point;)
  {
    points.push_back(point);

    switch(inputStream.peek()) case '\n': case EOF:
    {
      std::cout << ::getLineCount(points) << '\n';

      points.clear();
    }
  }
}
