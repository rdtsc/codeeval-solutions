#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <type_traits>
#include <unordered_set>

struct Word
{
  bool operator<(const std::string& other) const
  {
    return (this->data < other);
  }

  const char* const data;
  unsigned connections;
};

static bool hasConnection(const std::string& lhs, const std::string& rhs)
{
  // Words are considered friends if their edit distance is exactly one. Words
  // are their own friends in the context of a social circle.

  const auto lhsSize = lhs.size(),
             rhsSize = rhs.size();

  const auto sizeDelta = (lhsSize > rhsSize) ?
                         (lhsSize - rhsSize) :
                         (rhsSize - lhsSize);

  if(sizeDelta > 1) return false;

  else if(lhsSize == rhsSize)
  {
    if(lhs == rhs) return true;

    bool foundMismatch = false;

    for(auto i = decltype(lhsSize){0}; i < lhsSize; ++i)
    {
      if(lhs[i] != rhs[i])
      {
        if(foundMismatch) return false;

        foundMismatch = true;
      }
    }

    return foundMismatch;
  }

  const auto maxWidth = std::min(lhsSize, rhsSize);

  const auto mismatchIndex = [&]
  {
    std::remove_cv<decltype(maxWidth)>::type i = 0;

    for(;i < maxWidth; ++i) if(lhs[i] != rhs[i]) break;

    return i;
  }();

  const auto& a = (lhsSize > rhsSize) ? (rhs) : (lhs),
            & b = (lhsSize > rhsSize) ? (lhs) : (rhs);

  for(auto i = mismatchIndex; i < maxWidth; ++i)
  {
    if(a[i] != b[i + 1]) return false;
  }

  return true;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  #ifdef GENERATE_LUT
  // The word list is static, so may as well pre-compute (hackishly) the
  // results off-line.
  static const char* const words[] =
  {
    #include "words.dat"
  };

  std::size_t wordIndex = 0;

  for(std::string word : words)
  {
    std::unordered_set<std::string> friends;
    std::queue<std::string> queue;

    for(queue.push(word); !queue.empty();)
    {
      word = queue.front();
      queue.pop();

      for(const auto candidate : words) if(::hasConnection(word, candidate))
      {
        if(friends.find(candidate) == friends.cend())
        {
          friends.emplace(candidate);
          queue.push(candidate);
        }
      }
    }

    std::cout << "{\""
              << words[wordIndex++]
              << "\", "
              << friends.size()
              << "}, \n";
  }

  return 0;
  #endif

  static const Word lut[] =
  {
    #include "lut.dat"
  };

  static const auto lutSize = (sizeof lut / sizeof *lut);

  static const Word* const lutBegin = lut,
                   * const lutEnd   = (lut + lutSize);

  static const char inputTerminator[] = "END OF";

  for(std::string word; std::getline(inputStream, word);)
  {
    if(!word.compare(0, (sizeof inputTerminator - 1), inputTerminator)) break;

    const auto entry = std::lower_bound(lutBegin, lutEnd, word);

    assert(entry != lutEnd);

    std::cout << entry->connections << '\n';
  }
}
