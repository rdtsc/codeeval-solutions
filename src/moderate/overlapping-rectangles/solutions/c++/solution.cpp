#include <cassert>
#include <fstream>
#include <iostream>

template<typename T> struct Rectangle
{
  Rectangle() : x1(), y1(), x2(), y2() {}

  bool overlaps(const Rectangle& other) const
  {
    const bool noOverlap = this->x1 > other.x2 ||
                           this->x2 < other.x1 ||
                           this->y1 < other.y2 ||
                           this->y2 > other.y1;

    return !noOverlap;
  }

  friend std::istream& operator>>(std::istream& inputStream, Rectangle& rect)
  {
    static auto get = [&](T& destination)
    {
      inputStream >> destination;
      inputStream.ignore();
    };

    get(rect.x1);
    get(rect.y1);
    get(rect.x2);
    get(rect.y2);

    return inputStream;
  }

  /*
    (x1, y1) +------------+
             |            |
             +------------+ (x2, y2)
  */
  T x1, y1, x2, y2;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(Rectangle<int> a, b; inputStream >> a >> b;)
  {
    std::cout << (a.overlaps(b) ? "True" : "False") << '\n';
  }
}
