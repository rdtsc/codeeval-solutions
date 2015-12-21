#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

static std::istream& fastForward(std::istream& inputStream, const char target)
{
  static constexpr auto ignoreLimit =
    std::numeric_limits<std::streamsize>::max();

  return inputStream.ignore(ignoreLimit, target);
}

template<typename T> class CampaignStrategy
{
  public: explicit CampaignStrategy(std::istream& inputStream)
  {
    inputStream >> (*this);

    this->compile();
  }

  private: void compile()
  {
    std::size_t optimalMask = 0;
    auto optimalCost = std::numeric_limits<T>::max();

    const std::size_t maxMask = (1 << this->issues.size());

    // Try winning combinations of issues and retain the one with the lowest
    // overall price tag.
    for(std::size_t mask = 1; mask < maxMask; ++mask) if(this->canWin(mask))
    {
      const auto cost = this->getCost(mask);

      if(cost < optimalCost)
      {
        optimalMask = mask;
        optimalCost = cost;
      }
    }

    // Get issue names for our optimal strategy.
    this->forEachSlot(optimalMask, [&](const std::size_t i)
    {
      this->strategy.emplace_back(this->issues[i]);
    });

    // Issues should be presented in alphabetical order.
    std::sort(this->strategy.begin(), this->strategy.end());
  }

  private: bool canWin(const std::size_t selectors) const
  {
    T totalVotes = 0;

    for(const auto& region : this->regions)
    {
      const auto regionalVotes = [&]
      {
        T result = 0;

        this->forEachSlot(selectors, [&](const std::size_t i)
        {
          result += region.issues.at(this->issues[i].name);
        });

        return result;
      }();

      // As per the problem statement. Anything over 51% gives you all of
      // this region's votes.
      if((regionalVotes * 2) > region.voteCount)
      {
        totalVotes += region.voteCount;
      }
    }

    // As per the problem statement. Anything over 270 votes at the Electoral
    // College wins you the presidency.
    return (totalVotes >= 270);
  }

  private: T getCost(const std::size_t selectors) const
  {
    T result = 0;

    // Get the overall cost for our selected list of issues.
    this->forEachSlot(selectors, [&](const std::size_t i)
    {
      result += this->issues[i].value;
    });

    return result;
  }

  private: template<class F> static void forEachSlot(const std::size_t mask,
                                                     const F callback)
  {
    for(std::size_t i = mask, j = 0; i; i >>= 1, ++j)
    {
      if(i & 1) callback(j);
    }
  }

  private: friend std::istream& operator>>(std::istream& inputStream,
                                           CampaignStrategy& self)
  {
    const auto jumpTo =
      std::bind(::fastForward, std::ref(inputStream), std::placeholders::_1);

    std::size_t issueCount = 0;

    // Fetch issue count.
    if(!(jumpTo(':') >> issueCount)) assert(false);

    // Fetch main issues and their associated costs.
    {
      auto& issues = self.issues;

      issues.clear();
      issues.reserve(issueCount);

      jumpTo('\n');
      jumpTo('\n');

      const auto inputIterator  = std::istream_iterator<Issue>(inputStream);
      const auto outputIterator = std::back_inserter(issues);

      std::copy_n(inputIterator, issueCount, outputIterator);

      assert(std::all_of(issues.cbegin(), issues.cend(),
        std::mem_fun_ref(&Issue::isValid)));
    }

    // Fetch regions and their vote allocations.
    {
      auto& regions = self.regions;

      static const std::size_t expectedRegionCount = 51;

      regions.clear();
      regions.reserve(expectedRegionCount);

      jumpTo('\n');

      const auto inputIterator  = std::istream_iterator<Region>(inputStream);
      const auto outputIterator = std::back_inserter(regions);

      std::copy_n(inputIterator, expectedRegionCount, outputIterator);

      assert(std::all_of(regions.cbegin(), regions.cend(),
        std::mem_fun_ref(&Region::isValid)));
    }

    return inputStream;
  }

  private: friend std::ostream& operator<<(std::ostream& outputStream,
                                           const CampaignStrategy& self)
  {
    for(const auto& issue : self.strategy) outputStream << issue << '\n';

    return outputStream;
  }

  public: struct Issue
  {
    Issue() : value() {}

    bool isValid() const
    {
      return (!this->name.empty() && this->value);
    }

    bool operator<(const Issue& other) const
    {
      return (this->name < other.name);
    }

    friend std::istream& operator>>(std::istream& inputStream, Issue& self)
    {
      const auto jumpTo =
        std::bind(::fastForward, std::ref(inputStream), std::placeholders::_1);

      if(std::getline(inputStream, self.name, ':'))
      {
        inputStream >> self.value;
      }

      return jumpTo('\n');
    }

    friend std::ostream& operator<<(std::ostream& outputStream,
                                    const Issue& self)
    {
      return outputStream << self.name;
    }

    T value;
    std::string name;
  };

  public: struct Region
  {
    Region() : voteCount() {}

    bool isValid() const
    {
      return (!this->name.empty() && voteCount && !issues.empty());
    }

    friend std::istream& operator>>(std::istream& inputStream, Region& self)
    {
      const auto jumpTo =
        std::bind(::fastForward, std::ref(inputStream), std::placeholders::_1);

      // Name.
      if(!std::getline(inputStream, self.name)) return inputStream;

      // Total vote count.
      if(!(jumpTo(':') >> self.voteCount)) return inputStream;

      jumpTo('\n');

      self.issues.clear();

      for(Issue issue; inputStream >> issue;)
      {
        self.issues.emplace(issue.name, issue.value);

        const auto peek = inputStream.peek();

        if(peek == '\n' || peek == EOF) break;
      }

      return jumpTo('\n');
    }

    T voteCount;
    std::string name;
    std::unordered_map<std::string, T> issues;
  };

  private: std::vector<Issue> issues;
  private: std::vector<Region> regions;
  private: std::vector<Issue> strategy;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::cout << CampaignStrategy<unsigned>(inputStream);
}
