#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

struct Segment
{
  Segment() : begin(), length(), mismatch() {}

  bool operator<(const Segment& other) const
  {
    assert(this->length == other.length);

    // Alpha order breaks the tie.
    if(this->mismatch == other.mismatch)
    {
      const auto beginA = this->begin, endA = beginA + this->length,
                 beginB = other.begin, endB = beginB + this->length;

      return std::lexicographical_compare(beginA, endA, beginB, endB);
    }

    return (this->mismatch < other.mismatch);
  }

  std::string::iterator begin;
  std::size_t length;
  unsigned mismatch;
};

// Levenshtein distance.
static unsigned getPairMismatch(const std::string::const_iterator lhs,
                                const std::string::const_iterator rhs,
                                std::size_t length)
{
  using value_type = unsigned;

  static auto ops = []
  {
    static std::vector<value_type> result;

    result.resize(64);

    return result;
  }();

  ++length;

  for(auto i = decltype(length){1}; i < length; ++i)
  {
    ops[i] = i;
  }

  for(auto i = decltype(length){1}; i < length; ++i)
  {
    ops[0] = i;

    value_type prev = i - 1;

    for(auto j = decltype(length){1}; j < length; ++j)
    {
      const value_type del =  ops[  j  ] + 1,
                       ins =  ops[j - 1] + 1,
                       sub = (lhs[j - 1] != rhs[i - 1]) + prev;

      #define MIN(a, b, c) \
        ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

      const auto cache = ops[j];

      ops[j] = MIN(del, ins, sub);
      prev = cache;

      #undef MIN
    }
  }

  return ops[--length];
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::string segment, dna;
  unsigned mismatch = 0;

  for(std::vector<Segment> segments;
      inputStream >> segment >> mismatch >> dna;
      segments.clear())
  {
    const auto segmentLength = segment.size(),  dnaLength = dna.size();
    const auto segmentBegin  = segment.begin(), dnaBegin  = dna.begin();

    // Get the segments that satisfy the given mismatch threshold.
    for(std::size_t i = 0; i <= dnaLength - segmentLength; ++i)
    {
      const auto localMismatch =
        ::getPairMismatch(segmentBegin, dnaBegin + i, segmentLength);

      if(localMismatch <= mismatch)
      {
        Segment segment;

        segment.begin = dnaBegin + i;
        segment.length = segmentLength;
        segment.mismatch = localMismatch;

        segments.push_back(segment);
      }
    }

    std::sort(segments.begin(), segments.end());

    if(!segments.empty())
    {
      char segmentBuffer[64] = "";

      for(const auto& segment : segments)
      {
        std::copy(segment.begin,
                  segment.begin + segmentLength,
                  segmentBuffer);

        std::cout << segmentBuffer << ' ';
      }

      std::cout << '\n';
    }

    else std::cout << "No match\n";
  }
}
