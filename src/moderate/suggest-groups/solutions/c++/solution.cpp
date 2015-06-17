#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

struct User
{
  std::vector<std::reference_wrapper<const User>> buddies;
  std::vector<std::reference_wrapper<const std::string>> groups;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::map<std::string, User> userIndex;
  std::map<std::string, unsigned> groupIndex;
  std::istringstream tokenizer;

  const auto addUser = [&](const std::string& name) -> User&
  {
    auto entry = userIndex.find(name);

    if(entry == userIndex.cend())
    {
      entry = userIndex.emplace(name, User()).first;
    }

    return entry->second;
  };

  const auto addGroup = [&](const std::string& name) -> const std::string&
  {
    auto entry = groupIndex.find(name);

    if(entry == groupIndex.cend())
    {
      entry = groupIndex.emplace(name, 0).first;
    }

    return entry->first;
  };

  const auto resetGroupStats = [&]
  {
    for(auto& group : groupIndex) group.second = 0;
  };

  for(std::string chunk; inputStream;)
  {
    // Name.
    if(!std::getline(inputStream, chunk, ':')) break;

    User& user = addUser(chunk);

    // Friends.
    {
      std::getline(inputStream, chunk, ':');

      tokenizer.clear();

      for(tokenizer.str(chunk); std::getline(tokenizer, chunk, ',');)
      {
        user.buddies.push_back(addUser(chunk));
      }
    }

    // Groups.
    {
      std::getline(inputStream, chunk);

      tokenizer.clear();

      for(tokenizer.str(chunk); std::getline(tokenizer, chunk, ',');)
      {
        user.groups.push_back(addGroup(chunk));
      }
    }
  }

  const auto groupIndexBegin = groupIndex.cbegin(),
             groupIndexEnd   = groupIndex.cend();

  for(const auto& userEntry : userIndex)
  {
    const auto& userName  = userEntry.first;
    const auto& userData  = userEntry.second;
    const auto buddyCount = userData.buddies.size();

    resetGroupStats();

    // Fill the friend group membership frequency map.
    for(const User& buddy : userData.buddies)
    for(const std::string& group : buddy.groups)
    {
      ++groupIndex[group];
    }

    // Filter out the groups we already belong to.
    for(const std::string& group : userData.groups)
    {
      groupIndex[group] = 0;
    }

    using group_value_type = decltype(groupIndex)::value_type;

    const auto groupMeetsThreshold = [&](const group_value_type& group)
    {
      const auto memberCount = group.second;

      return (memberCount * 2 >= buddyCount);
    };

    auto group = std::find_if(groupIndexBegin,
                              groupIndexEnd,
                              groupMeetsThreshold);

    // Are there any groups to suggest?
    if(group == groupIndexEnd) continue;

    std::cout << userName << ':' << group->first;

    for(++group; group != groupIndexEnd; ++group)
    {
      if(groupMeetsThreshold(*group))
      {
        std::cout << ',' << group->first;
      }
    }

    std::cout << '\n';
  }
}
