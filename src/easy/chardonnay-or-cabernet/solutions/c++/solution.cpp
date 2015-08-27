#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

static void explode(const std::string& source,
                    std::vector<std::string>& bucket,
                    const char delimiter = ' ')
{
  bucket.clear();

  auto right = source.find(delimiter),
       left  = decltype(right){};

  // Should just return the head/tail index instead of a full substring, really.
  while(right != std::string::npos)
  {
    const auto fragmentWidth = (right - left);

    bucket.emplace_back(source.substr(left, fragmentWidth));

    left  = ++right;
    right = source.find(delimiter, left);
  }

  bucket.emplace_back(source.substr(left));
}

static void distill(std::vector<std::string>& pool,
                    std::string& fragments,
                    const char delimiter = '|')
{
  assert(pool.size() >= 3 && "<candidate>... <delimiter> <fragments>");

  fragments = pool.back();

  // Remove fragments from the right.
  pool.pop_back();

  assert(pool.back().front() == delimiter);

  // Remove delimiter from the right.
  pool.pop_back();
}

template<class Callback>
static std::size_t forEachMatch(const std::string& fragments,
                                const std::vector<std::string>& pool,
                                const Callback callback)
{
  std::size_t matchCount = 0;

  using frequency_map =
    std::array<std::size_t, (std::numeric_limits<char>::max() + 1)>;

  const auto getFrequencyMap = [](const std::string& source)
  {
    frequency_map result = {0};

    for(const auto c : source) ++result[c];

    return result;
  };

  const auto removeFragment = [](const char c, frequency_map& frequencies)
  {
    if(frequencies[c])
    {
      --frequencies[c];

      return true;
    }

    return false;
  };

  // Invoke supplied callback iff all fragments appear in the candidate chunk.
  for(const auto& chunk : pool)
  {
    auto frequencies = getFrequencyMap(chunk);

    const auto chunkContainsAllFragments = [&]
    {
      for(const auto fragment : fragments)
      {
        if(!removeFragment(fragment, frequencies)) return false;
      }

      return true;
    }();

    if(chunkContainsAllFragments)
    {
      ++matchCount;

      callback(chunk);
    }
  }

  return matchCount;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::vector<std::string> pool;

  for(std::string chunk; std::getline(inputStream, chunk); std::cout << '\n')
  {
    assert(std::all_of(chunk.cbegin(), chunk.cend(), [](const char c)
    {
      return (c == ' ') || (c == '|') || ::isalpha(c);
    }));

    ::explode(chunk, pool);
    ::distill(pool, chunk);

    const auto count = ::forEachMatch(chunk, pool, [](const std::string& match)
    {
      std::cout << match << ' ';
    });

    if(!count) std::cout << "False";
  }
}
