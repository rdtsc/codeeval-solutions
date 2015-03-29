#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

template<class Iterator>
static inline unsigned getDecodingCount(Iterator index, const Iterator end)
{
  if(std::distance(index, end) <= 1) return 1;

  else if((*index == '2' && *(index + 1) >= '7') || *index >= '3')
  {
    if(*index == '2' && *(index + 1) >= '7') ++index;

    return ::getDecodingCount(++index, end);
  }

  const auto lhs = ::getDecodingCount(++index, end),
             rhs = ::getDecodingCount(++index, end);

  return (lhs + rhs);
}

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
    std::cout << getDecodingCount(line.begin(), line.end()) << '\n';
  }
}
