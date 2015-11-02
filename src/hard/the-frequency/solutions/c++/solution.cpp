#include <array>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <numeric>

template<typename T> struct Point2d
{
  Point2d() : Point2d(0, 0) {}
  Point2d(const T& x, const T& y) : x(x), y(y) {}

  T x, y;
};

template<typename T> struct Segment2d
{
  Segment2d() {}

  Segment2d(const T& x0, const T& y0, const T& x1, const T& y1) :
    first(x0, y0), second(x1, y1) {}

  bool intersects(const Segment2d& other) const
  {
    using Point = const Point2d<T>&;

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

    Point a = this->first, b = this->second,
          c = other.first, d = other.second;

    return isIntersection(a, c, d, b, c, d) &&
           isIntersection(a, b, c, a, b, d);
  }

  Point2d<T> first, second;
};

template<typename T, std::size_t Size>
static unsigned getFrequency(const std::array<T, Size>& samples,
                             const unsigned sampleRate = 2e4)
{
  using value_type = long long;

  /*
    The sample's linear trend line has the form of: y = slope * x + intercept;

    Where:
    ------

    X = time slices;
    Y = amplitude samples;
    Z = sample count;

            Z * sum(X * Y) - sum(X) * sum(Y)
    slope = --------------------------------;
                Z * sum(X^2) - sum(X)^2


                sum(Y) - slope * sum(X)
    intercept = -----------------------;
                           Z
  */

  const auto amplitudeSum =
    std::accumulate(samples.cbegin(), samples.cend(), value_type());

  // Arithmetic sum.
  const value_type timeSum = (Size / 2) * (Size + 1);

  const auto trendSlope = [&]
  {
    const value_type a = [&]
    {
      value_type result = 0;
      std::size_t x = 0;

      for(const auto& value : samples)
      {
        result += (x++ * value);
      }

      return (Size * result);
    }();

    const value_type b = (amplitudeSum * timeSum);

    // Sum of squares.
    const value_type c = ((Size * Size) * (Size + 1) * (2 * Size + 1) / 6);

    const value_type d = (timeSum * timeSum);

    return static_cast<double>(a - b) / (c - d);
  }();

  const auto trendIntercept = ((amplitudeSum - (trendSlope * timeSum)) / Size);

  // Waveform trend line segment.
  const Segment2d<double> trend
  (
    /* x0 */ 0,
    /* y0 */ trendIntercept,
    /* x1 */ Size,
    /* y1 */ trendSlope * Size + trendIntercept
  );

  unsigned intersections = 0;

  // Walk through the waveform's immediate segment neighbor pairs and check
  // whether their line segment intersects the trend line. Unoptimized.
  for(std::size_t i = 0; i < (Size - 1); ++i)
  {
    // Current waveform segment.
    const Segment2d<double> waveformSegment
    (
      /* x0 */ i,
      /* y0 */ samples[i],
      /* x1 */ i + 1,
      /* y1 */ samples[i + 1]
    );

    if(trend.intersects(waveformSegment)) ++intersections;
  }

  const unsigned fHz = (intersections * (sampleRate / Size) / 2);

  // As per the problem statement, frequencies are multiples of 10.
  if(fHz % 10) return (fHz + 10 - (fHz % 10));

  return fHz;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::array<int, 2000> samples = {0};

  const auto samplesBegin = samples.begin(),
             samplesEnd   = samples.end();

  auto cursor = samplesBegin;

  for(decltype(samples)::value_type sample = 0; inputStream >> sample;)
  {
    assert(cursor != samplesEnd);

    *cursor++ = sample;

    switch(inputStream.peek()) case '\n': case EOF:
    {
      assert(cursor == samplesEnd);

      const auto fHz = ::getFrequency(samples);

      // As per the problem statement.
      assert(!(fHz % 10) && (fHz >= 150) && (fHz <= 2000));

      std::cout << fHz << '\n';

      cursor = samplesBegin;
    }
  }
}
