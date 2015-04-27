#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  bool isFirstSlice = true;
  std::size_t lastPortalIndex = 0;

  for(std::string line; std::getline(inputStream, line); isFirstSlice = false)
  {
    const auto lineSize = line.size();

    // Each track slice is at most 12 characters wide.
    assert(lineSize <= 12);

    // Each track slice has at most 2 portals.
    struct
    {
      std::size_t index;
      bool exists;
    } portals[2] = {};

    enum {CHECKPOINT, GATE};

    // Fetch portal positions in each track slice.
    for(std::size_t i = 0; i < lineSize; ++i)
    {
      switch(line[i]) case 'C': case '_':
      {
        const std::size_t type = (line[i] == 'C' ? CHECKPOINT : GATE);

        portals[type].index = i;
        portals[type].exists = true;

        if(portals[CHECKPOINT].exists && portals[GATE].exists) break;
      }
    }

    // Patch the current track slice with the appropriate turn symbol based on
    // the position of the previous portal. Checkpoints are given priority by
    // being checked for first.
    for(const auto& portal : portals) if(portal.exists)
    {
      const auto index = portal.index;

      #define IS_STRAIGHT (isFirstSlice || lastPortalIndex == index)
      #define IS_LEFT (lastPortalIndex > index)

      const char direction = (IS_STRAIGHT ? '|' : (IS_LEFT ? '/' : '\\'));

      #undef IS_LEFT
      #undef IS_STRAIGHT

      line[index] = direction;
      lastPortalIndex = index;

      break;
    }

    std::cout << line << '\n';
  }
}
