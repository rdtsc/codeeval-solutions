#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

struct Overlap
{
  Overlap() : value(), shouldSwap(), shouldMerge(), isOnLeft() {}

  Overlap(std::size_t value, bool swap = 0, bool merge = 0, bool left = 0) :
    value(value), shouldSwap(swap), shouldMerge(merge), isOnLeft(left)
  {
  }

  bool operator>(const Overlap& other) const
  {
    return (this->value > other.value);
  }

  std::size_t value;
  bool shouldSwap;
  bool shouldMerge;
  bool isOnLeft;
};

static void explode(const std::string& source,
                    std::vector<std::string>& bucket,
                    const char delimiter = ';')
{
  bucket.clear();

  auto right = source.find(delimiter),
       left  = decltype(right){};

  // Should be a bit leaner than a std::istringstream.
  while(right != std::string::npos)
  {
    const auto fragmentWidth = (right - left);

    bucket.emplace_back(source.substr(left, fragmentWidth));

    left  = ++right;
    right = source.find(delimiter, left);
  }

  bucket.emplace_back(source.substr(left));
}

// HACK: very naive search method. Should implement something faster if
// this becomes a bottleneck.
static Overlap getOverlap(const std::string& lhs, const std::string& rhs)
{
  static constexpr auto stringEnd = std::string::npos;

  const auto lhsSize = lhs.size(),
             rhsSize = rhs.size();

  if(lhs.find(rhs) != stringEnd) return rhsSize;
  if(rhs.find(lhs) != stringEnd) return Overlap(lhsSize, true);

  for(std::size_t offset = std::min(lhsSize, rhsSize); offset; --offset)
  {
    if(!lhs.compare((lhsSize - offset), stringEnd, rhs, 0, offset))
    {
      return Overlap(offset, false, true);
    }

    if(!rhs.compare((rhsSize - offset), stringEnd, lhs, 0, offset))
    {
      return Overlap(offset, false, true, true);
    }
  }

  return 0;
}

// Ditto regarding performance.
static void merge(std::string& lhs,
                  const std::string& rhs,
                  const Overlap& overlap)
{
  if(!overlap.shouldMerge)
  {
    if(overlap.shouldSwap) lhs = rhs;

    return;
  }

  else if(overlap.isOnLeft)
  {
    lhs.erase(0, overlap.value);
    lhs.insert(0, rhs);
  }

  else
  {
    lhs.erase(lhs.size() - overlap.value);
    lhs.append(rhs);
  }
}

static void reduce(std::vector<std::string>& fragments)
{
  if(fragments.size() <= 1) return;

  const auto fragmentsBegin = fragments.begin(),
             fragmentsEnd   = fragments.end();

  auto fragmentOne = fragmentsBegin,
       fragmentTwo = fragmentOne;

  Overlap maxOverlap;

  // Get a pair of fragments to smush together.
  for(auto i = fragmentsBegin; i != fragmentsEnd; ++i)
  for(auto j = std::next(i);   j != fragmentsEnd; ++j)
  {
    const auto thisOverlap = ::getOverlap(*i, *j);

    if(thisOverlap > maxOverlap)
    {
      fragmentOne = i;
      fragmentTwo = j;
      maxOverlap  = thisOverlap;
      break;
    }
  }

  ::merge(*fragmentOne, *fragmentTwo, maxOverlap);
  fragments.erase(fragmentTwo);
}

static void reconstruct(std::vector<std::string>& fragments)
{
  assert(!fragments.empty());

  while(fragments.size() > 1) ::reduce(fragments);
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::vector<std::string> fragments;

  for(std::string line; std::getline(inputStream, line);)
  {
    ::explode(line, fragments);
    ::reconstruct(fragments);

    std::cout << fragments.front() << '\n';
  }
}
