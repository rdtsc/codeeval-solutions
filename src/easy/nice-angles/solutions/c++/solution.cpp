#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>

template<typename T> struct Angle
{
  Angle() : value() {}

  T value;
};

template<typename T> static std::istream& operator>>(std::istream& inputStream,
                                                     Angle<T>& angle)
{
  return (inputStream >> angle.value);
}

template<typename T> static std::ostream& operator<<(std::ostream& outputStream,
                                                     const Angle<T>& angle)
{
  const auto origStreamFillChar = outputStream.fill();

  const auto n = angle.value;

  const unsigned degrees = n,
                 minutes = (n - degrees) * 60,
                 seconds = ((n - degrees) * 60 - minutes) * 60;

  outputStream.fill('0');

  outputStream << degrees << '.'
               << std::setw(2) << minutes << '\''
               << std::setw(2) << seconds << '"';

  outputStream.fill(origStreamFillChar);

  return outputStream;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(Angle<double> alpha; inputStream >> alpha; std::cout << alpha << '\n');
}
