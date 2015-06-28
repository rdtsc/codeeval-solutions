#include <cassert>
#include <fstream>
#include <iostream>

template<typename T> static T doEggStrengthTest(const T eggCount,
                                                const T floorCount)
{
  if(eggCount == 1 || floorCount == 1) return floorCount;

  else if(eggCount <= 0 || floorCount <= 0) return 0;

  // As described in:
  // https://en.wikipedia.org/wiki/Dynamic_programming#Egg_dropping_puzzle

  const T broken = doEggStrengthTest(eggCount - 1, floorCount - 1),
          intact = doEggStrengthTest(eggCount - 0, floorCount - 1);

  return (broken + intact + 1);
}

template<typename T> static T getTrialCount(const T eggCount,
                                            const T floorCount)
{
  assert((eggCount > 0) && (floorCount > 0));

  for(T result = 0; result < floorCount; ++result)
  {
    if(::doEggStrengthTest(eggCount, result) >= floorCount) return result;
  }

  return 0;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(unsigned eggCount = 0, floorCount = 0;
      inputStream >> eggCount >> floorCount;)
  {
    std::cout << ::getTrialCount(eggCount, floorCount) << '\n';
  }
}
