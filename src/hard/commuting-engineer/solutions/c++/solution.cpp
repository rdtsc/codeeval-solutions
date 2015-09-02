#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

template<typename T> struct Pin
{
  Pin() : id(), x(), y() {}

  T getDistance(const Pin& other) const
  {
    static constexpr auto meanPlanetaryDiameter = 12742.0;

    // As per: https://en.wikipedia.org/wiki/Haversine_formula
    const auto lhs = ::sin((other.x - this->x) / 2),
               rhs = ::sin((other.y - this->y) / 2);

    const auto phi    = (lhs * lhs),
               lambda = (rhs * rhs);

    return meanPlanetaryDiameter * ::asin
    (
      ::sqrt(phi + ::cos(this->x) * ::cos(other.x) * lambda)
    );
  }

  bool operator<(const Pin& other) const
  {
    return (this->id < other.id);
  }

  bool operator==(const Pin& other) const
  {
    return (this->id == other.id);
  }

  friend std::istream& operator>>(std::istream& inputStream, Pin& pin)
  {
    static constexpr auto ignoreLimit =
      std::numeric_limits<std::streamsize>::max();

    inputStream >> pin.id;

    inputStream.ignore(ignoreLimit, '(') >> pin.x;
    inputStream.ignore(ignoreLimit, ',') >> pin.y;

    static const auto radians = 0.01745329251994329576; // ~(Pi / 180)

    pin.x *= radians;
    pin.y *= radians;

    return inputStream.ignore(ignoreLimit, ')');
  }

  unsigned id;
  T x, y;
};

template<typename T, class PinCollection> struct Route
{
  Route() : length(std::numeric_limits<T>::infinity()) {}

  friend bool operator<(const T length, const Route& route)
  {
    return (length < route.length);
  }

  friend bool operator>(const T length, const Route& route)
  {
    return (route.length < length);
  }

  friend std::ostream& operator<<(std::ostream& outputStream,
                                  const Route& route)
  {
    for(const auto& pin : route.pins)
    {
      outputStream << pin->id << '\n';
    }

    return outputStream;
  }

  T length;
  PinCollection pins;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  using float_type = double;
  using Pin = Pin<float_type>;

  const auto pins = [&]
  {
    using input_iterator = std::istream_iterator<Pin>;
    using container_type = std::vector<input_iterator::value_type>;

    return container_type(input_iterator(inputStream), input_iterator());
  }();

  const auto pinCount = pins.size();

  // The supplied pins appear to be already sorted.
  assert(std::is_sorted(pins.cbegin(), pins.cend()));

  // Starting pin should have an ID of 1.
  assert(pins.front().id == 1);

  // Pins are supposed to be unique.
  assert(std::adjacent_find(pins.cbegin(), pins.cend()) == pins.cend());

  // Pins are supposed to have adjacent IDs.
  assert(std::all_of(pins.cbegin(), pins.cend(), [](const Pin& pin)
  {
    static auto previousId = decltype(pin.id){};

    return (++previousId == pin.id);
  }));

  // Cache pin distances relative to each other.
  const auto pinDistances = [&]
  {
    using row_type = std::vector<float_type>;

    const auto pinCount = (pins.size() + 1);

    std::vector<row_type> result(pinCount, row_type(pinCount));

    for(auto y = decltype(pinCount){1}; y < pinCount; ++y)
    for(auto x = decltype(pinCount){1}; x < pinCount; ++x)
    {
      result[y][x] = pins[y - 1].getDistance(pins[x - 1]);
    }

    return result;
  }();

  // Compile the initial route.
  auto route = [&]
  {
    std::vector<decltype(pins)::const_iterator> result;

    const auto pinsBegin = pins.cbegin(),
               pinsEnd   = pins.cend();

    for(auto i = pinsBegin; i != pinsEnd; ++i) result.push_back(i);

    return result;
  }();

  Route<float_type, decltype(route)> shortestRoute;

  // Only have to deal with ~10 locations, so brute force should be okay.
  for(auto routeBegin = std::next(route.begin()), routeEnd = route.end();;)
  {
    // Current variant route length.
    const auto routeLength = [&]
    {
      float_type result = 0;

      for(auto i = decltype(pinCount){}; i < (pinCount - 1); ++i)
      {
        result += pinDistances[route[i]->id][route[i + 1]->id];

        if(result > shortestRoute)
        {
          return std::numeric_limits<float_type>::infinity();
        }
      }

      return result;
    }();

    if(routeLength < shortestRoute)
    {
      shortestRoute.length = routeLength;
      shortestRoute.pins = route;
    }

    if(!std::next_permutation(routeBegin, routeEnd)) break;
  }

  std::cout << shortestRoute;
}
