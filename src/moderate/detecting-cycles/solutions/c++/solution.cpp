#include <algorithm>
#include <cassert>
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <unordered_set>
#include <vector>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Input terms are guaranteed to fall within: [0,99]
  using value_type = unsigned char;

  std::vector<value_type> values;
  std::unordered_set<value_type> history;
  std::deque<value_type> duplicates;

  for(unsigned term = 0; inputStream >> term;)
  {
    values.push_back(term);

    switch(inputStream.peek()) case '\n': case EOF:
    {
      for(auto i = values.crbegin(), iEnd = values.crend(); i != iEnd; ++i)
      {
        // Haven't seen this before.
        if(!history.count(*i))
        {
          history.insert(*i);
          continue;
        }

        const auto duplicatesEnd = duplicates.cend();

        // New term in the cycle.
        if(std::find(duplicates.cbegin(), duplicatesEnd, *i) == duplicatesEnd)
        {
          duplicates.push_front(*i);
        }
      }

      std::copy(duplicates.cbegin(),
                duplicates.cend(),
                std::ostream_iterator<decltype(term)>(std::cout, " "));

      values.clear();
      history.clear();
      duplicates.clear();

      std::cout << '\n';
    }
  }
}
