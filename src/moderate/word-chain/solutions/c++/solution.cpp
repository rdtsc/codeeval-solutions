#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

template<typename T> static std::vector<T> exclude(const T& target,
                                                   const std::vector<T>& pool)
{
  auto result = pool;

  result.erase(std::find(result.begin(), result.end(), target));

  return result;
}

static std::vector<std::string> getChain(const std::string& word,
                                         const std::vector<std::string>& pool)
{
  const auto options = [&]
  {
    std::vector<std::string> result;

    for(const auto& candidate : pool)
    {
      if(word.back() == candidate.front())
      {
        result.emplace_back(candidate);
      }
    }

    return result;
  }();

  if(options.empty()) return {word};

  std::vector<std::string> result;

  for(const auto& option : options)
  {
    const auto newChain = ::getChain(option, ::exclude(option, pool));

    if(newChain.size() > result.size()) result = newChain;
  }

  result.emplace(result.begin(), word);

  return result;
}

static std::size_t getLongestChainLength(const std::vector<std::string>& pool)
{
  std::size_t result = 0;

  for(const auto& word : pool)
  {
    const auto chainLength = ::getChain(word, ::exclude(word, pool)).size();

    if((chainLength > 1) && (chainLength > result)) result = chainLength;
  }

  return result;
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
  std::istringstream tokenizer;

  for(std::string line; std::getline(inputStream, line); std::cout << '\n')
  {
    pool.clear();
    tokenizer.clear();
    tokenizer.str(line);

    while(std::getline(tokenizer, line, ','))
    {
      pool.emplace_back(line);
    }

    const auto chainLength = ::getLongestChainLength(pool);

    if(!chainLength) std::cout << "None";

    else std::cout << chainLength;
  }
}
