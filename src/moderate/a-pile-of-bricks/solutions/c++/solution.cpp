#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>
#include <vector>

template<typename T> static inline T delta(const T(& pair)[2])
{
  return (pair[0] > pair[1]) ? (pair[0] - pair[1]) : (pair[1] - pair[0]);
}

template<typename T> struct Brick
{
  using value_type = T;

  Brick() : id(), width(), height(), depth() {}

  friend std::istream& operator>>(std::istream& inputStream, Brick& brick)
  {
    T x[2] = {}, y[2] = {}, z[2] = {};

    #define POINT(Index) x[(Index)] >> y[(Index)] >> z[(Index)]

    if(!(inputStream >> brick.id >> POINT(0) >> POINT(1))) return inputStream;

    #undef POINT

    brick.width  = ::delta(x);
    brick.height = ::delta(y);
    brick.depth  = ::delta(z);

    return inputStream;
  }

  unsigned id;
  T width, height, depth;
};

template<typename T> struct Hole
{
  using value_type = T;

  Hole() : width(), height() {}

  bool operator>=(const Brick<T>& brick) const
  {
    const auto w = brick.width,
               h = brick.height,
               d = brick.depth;

    // Basic rotations of the brick.
    struct Variant {T width, height;} const metrics[] =
    {
      {w, h}, {h, w},
      {w, d}, {d, w},
      {h, d}, {d, h}
    };

    // Try to fit the brick through the hole.
    return std::find_if(metrics, std::end(metrics), [&](const Variant& rotated)
    {
      return (this->width >= rotated.width && this->height >= rotated.height);
    }) != std::end(metrics);
  }

  friend std::istream& operator>>(std::istream& inputStream, Hole& hole)
  {
    T x[2] = {}, y[2] = {};

    if(!(inputStream >> x[0] >> y[0] >> x[1] >> y[1])) return inputStream;

    hole.width  = ::delta(x);
    hole.height = ::delta(y);

    return inputStream;
  }

  T width, height;
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

  // Add the following characters to the list of stream delimiters: "(),;[]|"
  tokenizer.imbue(std::locale(tokenizer.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    for(const auto c : "(),;[]|") categories[c] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  for(std::string line; std::getline(inputStream, line);)
  {
    tokenizer.clear();
    tokenizer.str(line);

    using Hole  = Hole<int>;
    using Brick = Brick<Hole::value_type>;

    for(Hole hole; tokenizer >> hole; std::cout << '\n')
    {
      bool foundBricks = false;

      // Looks like the brick IDs always start with 1, but are not guaranteed
      // to appear in ascending order afterwards.
      unsigned brickId = 1;

      for(Brick brick; tokenizer >> brick; ++brickId) if(hole >= brick)
      {
        if(foundBricks) std::cout << ',';

        std::cout << brickId;

        foundBricks = true;
      }

      if(!foundBricks) std::cout << '-';
    }
  }
}
