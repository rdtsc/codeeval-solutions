#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

struct TrackSegment
{
  TrackSegment() : length(), angle() {}

  friend std::istream& operator>>(std::istream& inputStream,
                                  TrackSegment& segment)
  {
    return (inputStream >> segment.length
                        >> segment.angle);
  }

  double length;
  unsigned angle;
};

struct Car
{
  Car() : id(), topSpeed(), accelerationRate(), decelerationRate() {}

  template<class TrackSegments>
  double getLapTime(const TrackSegments& track) const
  {
    const auto getTime = [&](const double speed, const double rate)
    {
      return ((this->topSpeed - speed) / rate);
    };

    const auto getDistance = [](const double speed,
                                const double time,
                                const double rate)
    {
      const auto lhs = (time * speed),
                 rhs = (rate * time * time / 2);

      const auto result = (lhs + rhs);

      return (speed < 0 ? -result : result);
    };

    double result = 0,
           speed  = 0;

    for(const auto& segment : track)
    {
      const auto accelerationTime =
        getTime(speed, this->accelerationRate);

      const auto accelerationSegment =
        getDistance(speed, accelerationTime, this->accelerationRate);

      const auto newSpeed = [&]
      {
        const auto result =
          (this->topSpeed * (1.0 - (segment.angle / 180.0)));

        return (speed = result);
      }();

      const auto decelerationTime =
        getTime(newSpeed, this->decelerationRate);

      const auto decelerationSegment =
        getDistance(-this->topSpeed, decelerationTime, this->decelerationRate);

      const auto straightawaySegment =
        (segment.length - accelerationSegment - decelerationSegment);

      const auto straightawayTime = (straightawaySegment / this->topSpeed);

      const auto segmentTime =
        (accelerationTime + straightawayTime + decelerationTime);

      result += segmentTime;
    }

    return result;
  }

  friend std::istream& operator>>(std::istream& inputStream, Car& car)
  {
    inputStream >> car.id
                >> car.topSpeed
                >> car.accelerationRate
                >> car.decelerationRate;

    car.topSpeed /= 3600;

    car.accelerationRate = (car.topSpeed / car.accelerationRate);
    car.decelerationRate = (car.topSpeed / car.decelerationRate);

    return inputStream;
  }

  unsigned id;
  double topSpeed;
  double accelerationRate;
  double decelerationRate;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Fetch track segments.
  const auto track = [&]
  {
    std::vector<TrackSegment> result;

    for(TrackSegment segment; inputStream >> segment;)
    {
      result.emplace_back(segment);

      const auto peek = inputStream.peek();

      if(peek == '\n' || peek == EOF) break;
    }

    return result;
  }();

  // Fetch car stats.
  const auto cars = [&]
  {
    std::vector<Car> result;

    for(Car car; inputStream >> car;) result.emplace_back(car);

    return result;
  }();

  // Run a simulated race.
  const auto leaderboard = [&]
  {
    std::multimap<double, unsigned> result;

    for(const auto& car : cars) result.emplace(car.getLapTime(track), car.id);

    return result;
  }();

  std::cout << std::fixed << std::setprecision(2);

  for(const auto& place : leaderboard)
  {
    std::cout << place.second << ' ' << place.first << '\n';
  }
}
