#include <cassert>
#include <cstdlib>
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

  for(std::string line; std::getline(inputStream, line);)
  {
    assert(line.find_first_not_of("01 |") == std::string::npos);

    char* cursor = nullptr;
    auto head = line.c_str();

    do
    {
      const auto value = [&]
      {
        auto result = std::strtoul(head, &cursor, 2);

        assert(result && (result <= 100));

        // Low upper bound, as per the problem statement.
        result ^= (result >> 1);
        result ^= (result >> 2);
        result ^= (result >> 4);

        return result;
      }();

      while(*cursor && (*cursor != '0') && (*cursor != '1')) ++cursor;

      std::cout << value << (*cursor ? " | " : "\n");

      head = cursor;
    }
    while(*cursor);
  }
}
