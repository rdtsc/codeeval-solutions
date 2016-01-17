#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

struct FragmentType
{
  using value_type = char;

  static constexpr value_type Tail = -1,
                              Body = +0,
                              Head = +1;
};

constexpr FragmentType::value_type FragmentType::Tail;
constexpr FragmentType::value_type FragmentType::Head;
constexpr FragmentType::value_type FragmentType::Body;

static void explode(const std::string& source,
                    std::vector<std::string>& bucket,
                    const char delimiter = '|')
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

template<typename Iterator, class Haystack>
static void stitch(const Iterator head,
                   Haystack& haystack,
                   std::string& tape)
{
  if(head == haystack.cend()) return;

  // Stack fragments top to bottom and record the chunks
  // just past the overlapping regions.
  for(auto& chain = head->second; !chain.empty();)
  {
    const auto nextFragment = haystack.find(chain.back()->first);

    chain.pop_back();

    stitch(nextFragment, haystack, tape);
  }

  tape += head->first.back();
}

static void implode(const std::vector<std::string>& haystack,
                    std::string& target)
{
  assert(!haystack.empty());

  const auto fragmentWidth = haystack.front().size();

  // NOTE: The constraints outlined in the problem statement are inaccurate.
  assert(fragmentWidth >= 5 && fragmentWidth <= 30);

  // Fragments should be of uniform width.
  assert(std::all_of(haystack.cbegin(), haystack.cend(),
    [&](const std::string& fragment)
  {
    return (fragment.size() == fragmentWidth);
  }));

  std::unordered_map<std::string, FragmentType::value_type> fragments;

  std::unordered_map<std::string,
                     std::vector<decltype(fragments)::const_iterator>> buckets;

  // Classify and group overlapping fragments.
  for(const auto& chunk : haystack)
  {
    const auto lhsChunk = chunk.substr(0, (fragmentWidth - 1)),
               rhsChunk = chunk.substr(1, (fragmentWidth - 1));

    const auto head = fragments.emplace(lhsChunk, FragmentType::Body);
    const auto tail = fragments.emplace(rhsChunk, FragmentType::Body);

    head.first->second += FragmentType::Head;
    tail.first->second += FragmentType::Tail;

    buckets[lhsChunk].emplace_back(tail.first);
  }

  const auto fragmentsBegin = fragments.cbegin(),
             fragmentsEnd   = fragments.cend();

  const auto isHeadFragment = [](const decltype(fragments)::value_type& entry)
  {
    return (entry.second == FragmentType::Head);
  };

  const auto isTailFragment = [](const decltype(fragments)::value_type& entry)
  {
    return (entry.second == FragmentType::Tail);
  };

  // Sanity check.
  assert(std::count_if(fragmentsBegin, fragmentsEnd, isHeadFragment) == 1);
  assert(std::count_if(fragmentsBegin, fragmentsEnd, isTailFragment) == 1);

  const auto leftmostFragment =
    std::find_if(fragmentsBegin, fragmentsEnd, isHeadFragment);

  const auto rightmostFragment =
    std::find_if(fragmentsBegin, fragmentsEnd, isTailFragment);

  // Start stitching from the head fragment.
  target = leftmostFragment->first;

  // Remove common overlap with the first fragment.
  target.pop_back();

  // Reconstruct body fragment tape.
  ::stitch(buckets.find(leftmostFragment->first), buckets, target);

  // Reverse the tape, less the head fragment.
  std::reverse(std::next(target.begin(), (fragmentWidth - 2)), target.end());

  // Tail fragment.
  target += rightmostFragment->first.back();
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
    assert((line.front() == '|') && (line.back() == '|'));

    // Remove terminal pipes.
    line.erase(0, 1);
    line.pop_back();

    ::explode(line, fragments);
    ::implode(fragments, line);

    std::cout << line << '\n';
  }
}
