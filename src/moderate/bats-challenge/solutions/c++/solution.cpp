#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  static constexpr unsigned wallTolerance = 6;

  std::vector<unsigned> batPositions;

  for(unsigned wireLength = 0, batTolerance = 0, existingBatCount = 0;
      inputStream >> wireLength >> batTolerance >> existingBatCount;)
  {
    unsigned roomLeft = 0;

    // Clean wire.
    if(!existingBatCount)
    {
      // Bats don't sit closer than 6 units to a building.
      assert(wireLength >= (wallTolerance * 2));

      roomLeft = ((wireLength - (wallTolerance * 2)) / batTolerance) + 1;
    }

    // Some bats are already on the wire.
    else
    {
      batPositions.clear();

      for(auto i = decltype(existingBatCount){}; i < existingBatCount; ++i)
      {
        decltype(batPositions)::value_type value = 0;

        inputStream >> value;

        batPositions.push_back(value);
      }

      const auto upperLimit = (wireLength - wallTolerance);

      // Find open room on the wire.
      for(auto i = wallTolerance; i <= upperLimit; i += batTolerance)
      {
        const bool foundRoom = [&]
        {
          for(const auto pos : batPositions)
          {
            const auto dist = ((pos > i) ? (pos - i) : (i - pos));

            // Are we within spec?
            if(dist < batTolerance)
            {
              i = pos;
              return false;
            }
          }

          return true;
        }();

        if(foundRoom) ++roomLeft;
      }
    }

    std::cout << roomLeft << '\n';
  }
}
