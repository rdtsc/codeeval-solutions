#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::string emailSuffix;

  const auto index = [&]
  {
    std::unordered_map<std::string, std::set<std::string>> index;

    for(std::string line, lhs, rhs; std::getline(inputStream, line);)
    {
      // Fetch initiating user's name.
      {
        const auto lhsMarker = line.find_first_of('\t'),
                   rhsMarker = line.find_first_of('@');

        assert(lhsMarker != std::string::npos);
        assert(rhsMarker != std::string::npos);

        lhs = line.substr((lhsMarker + 1), (rhsMarker - lhsMarker - 1));
      }

      // Fetch target user's name.
      {
        const auto lhsMarker = line.find_last_of('\t'),
                   rhsMarker = line.find_last_of('@');

        assert(lhsMarker != std::string::npos);
        assert(rhsMarker != std::string::npos);

        rhs = line.substr((lhsMarker + 1), (rhsMarker - lhsMarker - 1));

        // Looks like all emails belong to the same domain.
        if(emailSuffix.empty()) emailSuffix = line.substr(rhsMarker);
      }

      // Save the interaction.
      index[lhs].emplace(rhs);
    }

    assert(!emailSuffix.empty() && (emailSuffix.front() == '@'));

    std::vector<decltype(index)::mapped_type> result;

    // Flatten the interaction table, ignoring insignificant entries.
    for(auto& entry : index) if(entry.second.size() > 1)
    {
      entry.second.emplace(entry.first);
      result.emplace_back(entry.second);
    }

    return result;
  }();

  std::map<std::set<std::string>, unsigned> cliques;

  // Inject existing interactions.
  for(const auto& clique : index) ++cliques[clique];

  // Find more. The dataset is small; dumb brute force should be fast enough.
  {
    const auto indexBegin = index.cbegin(),
               indexEnd   = index.end();

    decltype(cliques)::key_type intersection;

    for(auto i = indexBegin  ; i != indexEnd; ++i)
    for(auto j = std::next(i); j != indexEnd; ++j)
    {
      intersection.clear();

      const auto outputIterator =
        std::inserter(intersection, intersection.begin());

      std::set_intersection(i->cbegin(), i->cend(),
                            j->cbegin(), j->cend(),
                            outputIterator);

      if(intersection.size() > 2) ++cliques[intersection];
    }
  }

  // Dump clusters above the set size threshold.
  for(const auto& clique : cliques) if(clique.second > 2)
  {
    const auto namesBegin = clique.first.cbegin(),
               namesEnd   = clique.first.cend();

    std::cout << *namesBegin << emailSuffix;

    for(auto i = std::next(namesBegin); i != namesEnd; ++i)
    {
      std::cout << ", " << *i << emailSuffix;
    }

    std::cout << '\n';
  }
}
