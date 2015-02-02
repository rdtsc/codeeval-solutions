#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>

template<typename T> class InputLine
{
  public: using value_type = T;

  private: static constexpr auto ignoreLimit =
    std::numeric_limits<std::streamsize>::max();

  public: struct Point
  {
    friend std::istream& operator>>(std::istream& inputStream, Point& point)
    {
      inputStream.ignore(InputLine::ignoreLimit, '(');
      inputStream >> point.x;
      inputStream.ignore(InputLine::ignoreLimit, ',');
      return inputStream >> point.y;
    }

    T x, y;
  };

  public: struct Circle
  {
    friend std::istream& operator>>(std::istream& inputStream, Circle& circle)
    {
      inputStream >> circle.center;
      inputStream.ignore(InputLine::ignoreLimit, ':');
      return inputStream >> circle.radius;
    }

    Point center;
    T radius;
  };

  private: friend std::istream& operator>>(std::istream& inputStream,
                                           InputLine& input)
  {
    return inputStream >> input.circle >> input.point;
  }

  public: Circle circle;
  public: Point point;
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

  using Input = InputLine<double>;

  for(Input line; inputStream >> line;)
  {
    const auto& center = line.circle.center,
              & point  = line.point;

    const auto deltaX = point.x - center.x,
               deltaY = point.y - center.y;

    // Work out the distance between the circle's center and the test point.
    const Input::value_type pointDistance = ::sqrt(deltaX * deltaX +
                                                   deltaY * deltaY);

    // The test point lies within the circle if the distance between it and
    // the circle's center is less than the circle's radius.
    std::cout << (pointDistance < line.circle.radius) << '\n';
  }
}
