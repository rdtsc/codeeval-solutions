#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  unsigned angle;
  double length;
} TrackSegment;

typedef struct
{
  unsigned id;
  double topSpeed;
  double accelerationRate;
  double decelerationRate;
} Car;

typedef struct
{
  unsigned id;
  double time;
} Standing;

static inline int fastestLapOrdering(const void* const lhs,
                                     const void* const rhs)
{
  assert(lhs && rhs);

  const double x = ((const Standing*)lhs)->time,
               y = ((const Standing*)rhs)->time;

  return (x < y) ? (-1) : (x > y);
}

static inline double getTime(const Car* const car,
                             const double speed,
                             const double rate)
{
  assert(car);

  return ((car->topSpeed - speed) / rate);
}

static inline double getDistance(const double speed,
                                 const double time,
                                 const double rate)
{
  const double lhs = (time * speed),
               rhs = (rate * time * time / 2);

  const double result = (lhs + rhs);

  return (speed < 0 ? -result : result);
}

static Standing carGetLapTime(const TrackSegment* const segments,
                              const size_t segmentCount,
                              const Car* const car)
{
  assert(segments && segmentCount && car);

  double chronograph = 0,
         speedometer = 0;

  for(size_t i = 0; i < segmentCount; ++i)
  {
    const TrackSegment* const segment = &segments[i];

    const double accelerationTime =
      getTime(car, speedometer, car->accelerationRate);

    const double accelerationSegment =
      getDistance(speedometer, accelerationTime, car->accelerationRate);

    const double newSpeed =
      (car->topSpeed * (1.0 - (segment->angle / 180.0)));

    speedometer = newSpeed;

    const double decelerationTime =
      getTime(car, newSpeed, car->decelerationRate);

    const double decelerationSegment =
      getDistance(-car->topSpeed, decelerationTime, car->decelerationRate);

    const double straightawaySegment =
      (segment->length - accelerationSegment - decelerationSegment);

    const double straightawayTime = (straightawaySegment / car->topSpeed);

    const double segmentTime =
      (accelerationTime + straightawayTime + decelerationTime);

    chronograph += segmentTime;
  }

  const Standing result = {.id = car->id, .time = chronograph};

  return result;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[384] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  TrackSegment segments[128] = {{0}};
  size_t segmentCount = 0;

  // Fetch track segments.
  {
    char lineBuffer[1024] = "", * data = lineBuffer;

    fgets(lineBuffer, sizeof lineBuffer, inputStream);

    TrackSegment* segment = segments;

    #define ARGUMENT_PACK &segment->length, &segment->angle, &bytesRead

    for(int bytesRead = 0;
        sscanf(data, "%lf %u %n", ARGUMENT_PACK) == 2;
        data += bytesRead)
    {
      assert(segmentCount < (sizeof segments / sizeof *segments));

      segment = &segments[++segmentCount];
    }

    #undef ARGUMENT_PACK
  }

  Car cars[64] = {{0}};
  size_t carCount = 0;

  // Fetch car stats.
  {
    Car* car = cars;

    #define ARGUMENT_PACK \
      &car->id, &car->topSpeed, &car->accelerationRate, &car->decelerationRate

    while(fscanf(inputStream, "%u %lf %lf %lf", ARGUMENT_PACK) == 4)
    {
      assert(carCount < (sizeof cars / sizeof *cars));

      car->topSpeed /= 3600;

      car->accelerationRate = (car->topSpeed / car->accelerationRate);
      car->decelerationRate = (car->topSpeed / car->decelerationRate);

      car = &cars[++carCount];
    }

    #undef ARGUMENT_PACK
  }

  Standing leaderboard[64] = {{0}};

  assert(carCount <= (sizeof leaderboard / sizeof *leaderboard));

  // Run a simulated race. Ignoring ties for simplicity's sake.
  {
    for(size_t i = 0; i < carCount; ++i)
    {
      leaderboard[i] = carGetLapTime(segments, segmentCount, &cars[i]);
    }

    qsort(leaderboard, carCount, sizeof *leaderboard, fastestLapOrdering);
  }

  for(size_t i = 0; i < carCount; ++i)
  {
    printf("%u %.2lf\n", leaderboard[i].id, leaderboard[i].time);
  }

  // The CRT takes care of cleanup.
}
