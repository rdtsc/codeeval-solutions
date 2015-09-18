#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>

template<typename T> struct Timestamp
{
  Timestamp() : h(), m(), s(), value() {}

  bool operator>(const Timestamp& other) const
  {
    return (this->value > other.value);
  }

  friend std::istream& operator>>(std::istream& inputStream,
                                  Timestamp& timestamp)
  {
    static constexpr auto ignoreLimit =
      std::numeric_limits<std::streamsize>::max();

    // The supplied timestamps are in ISO 8601/Extended format.
    inputStream >> timestamp.h; inputStream.ignore(ignoreLimit, ':');
    inputStream >> timestamp.m; inputStream.ignore(ignoreLimit, ':');
    inputStream >> timestamp.s;

    timestamp.value = (timestamp.h * 3600) +
                      (timestamp.m *   60) +
                      (timestamp.s *    1);

    return inputStream;
  }

  friend std::ostream& operator<<(std::ostream& outputStream,
                                  const Timestamp& timestamp)
  {
    #define CHUNK(Chunk) std::setw(2) << std::setfill('0') << (Chunk)

    return outputStream << CHUNK(timestamp.h) << ':'
                        << CHUNK(timestamp.m) << ':'
                        << CHUNK(timestamp.s);

    #undef CHUNK
  }

  T h, m, s;
  T value;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  using Timestamp = Timestamp<unsigned>;

  std::vector<Timestamp> timestamps;

  const auto sortPredicate = std::greater<Timestamp>();

  for(Timestamp timestamp; inputStream >> timestamp;)
  {
    timestamps.emplace_back(timestamp);

    switch(inputStream.peek()) case '\n': case EOF:
    {
      std::sort(timestamps.begin(), timestamps.end(), sortPredicate);

      for(const auto& entry : timestamps)
      {
        std::cout << entry << ' ';
      }

      std::cout << '\n';

      timestamps.clear();
    }
  }
}
