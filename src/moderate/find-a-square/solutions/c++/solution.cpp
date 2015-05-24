#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
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

  double getDistance(const Point2d& other) const
  {
    return ::sqrt(::pow(other.x - this->x, 2) +
                  ::pow(other.y - this->y, 2));
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

  std::boolalpha(std::cout);

  for(Point2d<int> points[4]; inputStream >> points[0] >> points[1] >>
                                             points[2] >> points[3];)
  {
    const auto distances = [&]
    {
      std::array<double, 6> distances =
      {
        #define DISTANCE(A, B) points[(A)].getDistance(points[(B)])

        /* A */ DISTANCE(0, 1), // ┌──A──┐  ┌──B──┐  ┌──C──┐
        /* A */ DISTANCE(0, 2), // │0---1│  │0   1│  │0   1│
        /* A */ DISTANCE(0, 3), // │| \  │  │  / |│  │     │
        /* B */ DISTANCE(1, 2), // │|  \ │  │ /  |│  │     │
        /* B */ DISTANCE(1, 3), // │2   3│  │2   3│  │2---3│
        /* C */ DISTANCE(2, 3)  // └─────┘  └─────┘  └─────┘

        /*
          - Faces:       {(0,1), (0,2), (1,3), (2,3)}
          - Hypotenuses: {(0,3), (1,2)}
        */

        #undef DISTANCE
      };

      // Group face lengths and hypotenuses.
      std::sort(distances.begin(), distances.end());

      return distances;
    }();

    static constexpr std::size_t distanceCount = distances.size();

    assert(distanceCount == 6);

    const bool isSquare = [&]
    {
      // Hypotenuses should be identical.
      bool result = (distances[4] == distances[5]);

      const auto faceLength = distances[0];

      // Face lengths should be identical.
      if(result) for(std::size_t i = 0; i < (distanceCount - 2); ++i)
      {
        if(!distances[i] || distances[i] != faceLength) return false;
      }

      return result;
    }();

    std::cout << isSquare << '\n';
  }
}
