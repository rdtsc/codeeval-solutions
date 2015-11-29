#include <cassert>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <set>
#include <sstream>
#include <type_traits>
#include <vector>

template<typename T> struct Point
{
  Point() : x(), y() {}
  Point(const T& x, const T& y) : x(x), y(y) {}

  bool inPolygon(const std::vector<Point>& bounds) const
  {
    const auto pointCount = bounds.size();

    unsigned intersections = 0;

    // If a ray cast from our position intersects an odd number of polygon edges
    // then we are located within the polygon. Implemented as described in Q2.3
    // under 2D Polygon Computations in the comp.graphics.algorithms FAQ.
    for(std::size_t i = 0, j = (pointCount - 1); i < pointCount; j = i++)
    {
      const auto& x1 = bounds[i].x, & y1 = bounds[i].y,
                & x2 = bounds[j].x, & y2 = bounds[j].y;

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

  bool operator<(const Point& other) const
  {
    const auto& x1 = this->x, & y1 = this->y,
              & x2 = other.x, & y2 = other.y;

    // A la std::pair.
    return ((x1 < x2) || (!(x2 < x1) && (y1 < y2)));
  }

  friend std::istream& operator>>(std::istream& inputStream, Point& point)
  {
    return (inputStream >> point.x >> point.y);
  }

  T x, y;
};

template<typename T> using Polygon = std::vector<Point<T>>;

template<typename T> std::istream& operator>>(std::istream& inputStream,
                                              Polygon<T>& polygon)
{
  polygon.clear();

  for(typename Polygon<T>::value_type point; inputStream >> point;)
  {
    polygon.emplace_back(point);

    const auto peek = inputStream.peek();

    if(peek == '\n' || peek == EOF) break;
  }

  return inputStream;
}

template<typename T> struct Building
{
  friend std::istream& operator>>(std::istream& inputStream,
                                  Building& building)
  {
    return (inputStream >> building.id >> building.bounds);
  }

  std::string id;
  Polygon<T> bounds;
};

template<typename T> struct Snapshot
{
  friend std::istream& operator>>(std::istream& inputStream,
                                  Snapshot& snapshot)
  {
    if(!(inputStream >> snapshot.position)) return inputStream;

    snapshot.angles.clear();

    std::string mac;
    T azimuth = {};

    while(inputStream >> mac >> azimuth)
    {
      snapshot.angles.emplace_back(azimuth);

      const auto peek = inputStream.peek();

      if(peek == '\n' || peek == EOF) break;
    }

    static constexpr auto radians = (std::acos(-1) / 180);

    // As per the problem statement, 0° azimuth is equivalent to the +Y axis.
    for(auto& azimuth : snapshot.angles)
    {
      azimuth = (-azimuth + 90) * radians;
    }

    return inputStream;
  }

  Point<T> position;
  std::vector<T> angles;
};

template<typename T> struct Segment
{
  Segment(const T& x1, const T& y1, const T& x2, const T& y2) :
    a(x1, y1), b(x2, y2) {}

  bool intersects(const Segment& other) const
  {
    using Point = const Point<T>&;

    static const auto isCounterclockwise = [](Point a, Point b, Point c)
    {
      return ((c.y - a.y) * (b.x - a.x)) >
             ((b.y - a.y) * (c.x - a.x));
    };

    static const auto isIntersection = [](Point a, Point b, Point c,
                                          Point d, Point e, Point f)
    {
      return (isCounterclockwise(a, b, c) !=
              isCounterclockwise(d, e, f));
    };

    Point a(this->a), b(this->b),
          c(other.a), d(other.b);

    return isIntersection(a, c, d, b, c, d) &&
           isIntersection(a, b, c, a, b, d);
  }

  Point<T> a, b;
};

template<typename T> struct SiteSurvey
{
  static_assert(std::is_floating_point<T>::value,
    "Underlying type should be floating point, as per the problem's input.");

  explicit SiteSurvey(std::istream& inputStream)
  {
    std::istringstream tokenizer;

    // Add semicolons to the list of stream delimiters.
    tokenizer.imbue(std::locale(tokenizer.getloc(), []
    {
      using facet = std::ctype<char>;

      const auto classicTableStart = facet::classic_table(),
                 classicTableEnd   = classicTableStart + facet::table_size;

      static std::vector<facet::mask> categories(classicTableStart,
                                                 classicTableEnd);

      categories[';'] |= facet::space;

      // Ref-counted upstream.
      return new facet(categories.data());
    }()));

    std::string line;

    #define APPEND_TO(Target) do {                 \
      tokenizer.clear();                           \
      tokenizer.str(line);                         \
      typename decltype(Target)::value_type value; \
      tokenizer >> value;                          \
      Target.emplace_back(value);                  \
    } while(false)

    // Fetch buildings. An empty line separates the building map
    // from the radar snapshots.
    while(std::getline(inputStream, line) && !line.empty())
    {
      APPEND_TO(this->buildings);
    }

    // Fetch radar snapshots.
    while(std::getline(inputStream, line)) APPEND_TO(this->snapshots);

    #undef APPEND_TO
  }

  friend std::ostream& operator<<(std::ostream& outputStream,
                                  const SiteSurvey& site)
  {
    std::vector<Segment<T>> segments;

    // Create signal return segments.
    for(const auto& snapshot : site.snapshots)
    {
      const auto& x1 = snapshot.position.x,
                & y1 = snapshot.position.y;

      for(const auto& angle : snapshot.angles)
      {
        static constexpr T segmentLength = 1e3;

        const auto x2 = (x1 + (segmentLength * std::cos(angle))),
                   y2 = (y1 + (segmentLength * std::sin(angle)));

        segments.emplace_back(x1, y1, x2, y2);
      }
    }

    std::map<Point<T>, std::size_t> intersections;

    const auto grind = [](const T& value)
    {
      return (std::round(value * 1e3) / 1e3);
    };

    const auto segmentCount = segments.size();

    // Get signal return segment intersections that are inside buildings.
    for(std::size_t i = (0 + 0); i < segmentCount; ++i)
    for(std::size_t j = (i + 1); j < segmentCount; ++j)
    {
      if(segments[i].intersects(segments[j]))
      {
        const auto& a = segments[i],
                  & b = segments[j];

        const auto slopeA = ((a.b.y - a.a.y) / (a.b.x - a.a.x)),
                   slopeB = ((b.b.y - b.a.y) / (b.b.x - b.a.x));

        const auto interceptA = (a.a.y - (slopeA * a.a.x)),
                   interceptB = (b.b.y - (slopeB * b.b.x));

        const auto x = grind((interceptB - interceptA) / (slopeA - slopeB)),
                   y = grind((slopeA * x) + interceptA);

        const Point<T> point(x, y);

        // Check if we're inside a building. TODO: Should just cache the
        // building ID at this point.
        if(x >= 0 && y >= 0) for(const auto& building : site.buildings)
        {
          if(point.inPolygon(building.bounds))
          {
            ++intersections[point];
            break;
          }
        }
      }
    }

    std::set<std::string> buildingIds;

    // Save building IDs that have signal returns converge inside them.
    for(const auto& intersection : intersections) if(intersection.second > 1)
    {
      for(const auto& building : site.buildings)
      {
        if(intersection.first.inPolygon(building.bounds))
        {
          buildingIds.insert(building.id);
          break;
        }
      }
    }

    for(const auto& id : buildingIds) outputStream << id << '\n';

    return outputStream;
  }

  std::vector<Building<T>> buildings;
  std::vector<Snapshot<T>> snapshots;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::cout << SiteSurvey<float>(inputStream);
}
