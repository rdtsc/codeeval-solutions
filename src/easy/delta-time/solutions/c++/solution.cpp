#include <algorithm>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>

template<typename T = unsigned> struct Timestamp
{
  Timestamp() : Timestamp(0, 0, 0) {}
  Timestamp(const T h, const T m, const T s) : h(h), m(m), s(s) {}

  Timestamp operator-(const Timestamp& other) const
  {
    const T thisSeconds  = ((this->h * 3600) + (this->m * 60) + this->s),
            otherSeconds = ((other.h * 3600) + (other.m * 60) + other.s);

    const T deltaSeconds = std::max(thisSeconds, otherSeconds) -
                           std::min(thisSeconds, otherSeconds);

    return Timestamp
    (
      (deltaSeconds / 3600),    // HH
      (deltaSeconds / 60) % 60, // MM
      (deltaSeconds % 60)       // SS
    );
  }

  T h, m, s;
};

template<typename T> std::istream& std::operator>>(std::istream& inputStream,
                                                   Timestamp<T>& timestamp)
{
  static constexpr auto ignoreLimit =
    std::numeric_limits<std::streamsize>::max();

  // The supplied timestamps are in ISO 8601/Extended format.
  inputStream >> timestamp.h; inputStream.ignore(ignoreLimit, ':');
  inputStream >> timestamp.m; inputStream.ignore(ignoreLimit, ':');

  return (inputStream >> timestamp.s);
}

template<typename T> std::ostream& operator<<(std::ostream& outputStream,
                                              const Timestamp<T>& timestamp)
{
  #define CHUNK(Chunk) std::setw(2) << std::setfill('0') << (Chunk)

  return outputStream << CHUNK(timestamp.h) << ':'
                      << CHUNK(timestamp.m) << ':'
                      << CHUNK(timestamp.s);

  #undef CHUNK
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(Timestamp<> lhs, rhs; inputStream >> lhs >> rhs;)
  {
    std::cout << (lhs - rhs) << '\n';
  }
}
