#include <cassert>
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

  for(std::string line; std::getline(inputStream, line); std::cout << '\n')
  {
    if(line.size() <= 55) std::cout << line;

    else
    {
      // Find the last space and ignore everything after it (inclusive).
      const auto trimPosition = [&]() -> std::string::size_type
      {
        // 40 chars is our text length limit.
        static constexpr std::string::size_type chunkSize = 40;

        static_assert(chunkSize > 0, "Chunk size should be greater than zero.");

        const auto lastSpace = line.rfind(' ', chunkSize - 1);

        return (lastSpace == std::string::npos) ? (chunkSize) : (lastSpace);
      }();

      std::cout << line.substr(0, trimPosition) << "... <Read More>";
    }
  }
}
