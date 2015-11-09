#include <algorithm>
#include <cassert>
#include <cstddef>
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

  T x, y;
};

struct Bridge
{
  Bridge() : id() {}

  bool intersects(const Bridge& other) const
  {
    using Point = const Point2d<decltype(this->a)::value_type>&;

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

    Point a = this->a, b = this->b,
          c = other.a, d = other.b;

    return isIntersection(a, c, d, b, c, d) &&
           isIntersection(a, b, c, a, b, d);
  }

  friend std::istream& operator>>(std::istream& inputStream, Bridge& bridge)
  {
    return (inputStream >> bridge.id >> bridge.a >> bridge.b);
  }

  unsigned id;
  Point2d<double> a, b;
};

template<class Container, class Callback>
static void forEachCombination(const Container& pool,
                               const std::size_t select,
                               const Callback callback)
{
  const auto poolSize = pool.size();

  std::vector<std::size_t> combination;
  std::vector<bool> mask(select, true);

  for(mask.resize(poolSize); true; combination.clear())
  {
    for(auto i = decltype(poolSize){}; i < poolSize; ++i) if(mask[i])
    {
      combination.push_back(i);
    }

    if(!callback(combination)) return;
    if(!std::prev_permutation(mask.begin(), mask.end())) return;
  }
}

template<class BridgeContainer>
static std::vector<unsigned> getOptimalBridgeSet(const BridgeContainer& bridges)
{
  assert(!bridges.empty());

  // Bridge IDs appear to start from 1 and are sequential. Caveat: table rows
  // are boolean specializations of std::vector.
  const auto intersections = [&]
  {
    using row_type = std::vector<bool>;

    const auto tableSize = (bridges.size() + 1);

    std::vector<row_type> results(tableSize, row_type(tableSize));

    for(const auto& a : bridges) for(const auto& b : bridges)
    {
      if(a.intersects(b))
      {
        results[a.id][b.id] = results[b.id][a.id] = true;
      }
    }

    return results;
  }();

  std::vector<unsigned> results, layout;

  // Find the largest combination of non-intersecting bridges.
  {
    const auto canBuild = [&](const Bridge& candidate)
    {
      for(const auto& bridge : layout)
      {
        if(intersections[bridge][candidate.id]) return false;
      }

      return true;
    };

    const auto bridgeCount = bridges.size();

    for(auto i = decltype(bridgeCount){1}; i <= bridgeCount; ++i)
    {
      using index_container = std::vector<std::size_t>;

      ::forEachCombination(bridges, i, [&](const index_container& combination)
      {
        layout.clear();

        for(const auto index : combination)
        {
          if(canBuild(bridges[index])) layout.push_back(index + 1);

          else return true;
        }

        if(layout.size() > results.size()) results = layout;

        return false;
      });
    }
  }

  // Bridge IDs should be in ascending order, as per the problem statement.
  std::sort(results.begin(), results.end());

  return results;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Add the following characters to the list of stream delimiters: ":,()[]"
  inputStream.imbue(std::locale(inputStream.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    for(const auto c : ":,()[]") if(c) categories[c] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  const auto bridges = [&]
  {
    std::vector<Bridge> results;

    using input_iterator =
      std::istream_iterator<decltype(results)::value_type>;

    const auto outputIterator = std::back_inserter(results);

    std::copy(input_iterator(inputStream), input_iterator(), outputIterator);

    return results;
  }();

  for(const auto& bridgeId : ::getOptimalBridgeSet(bridges))
  {
    std::cout << bridgeId << '\n';
  }
}
