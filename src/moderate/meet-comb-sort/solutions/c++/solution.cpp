#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

template<class Iterator> static std::size_t combSort(const Iterator first,
                                                     const Iterator last,
                                                     const float factor = 1.25f)
{
  std::size_t result = 0;

  for(auto margin = std::distance(first, last);; ++result)
  {
    if(margin > 1) margin /= factor;

    auto i = first,
         j = std::next(i, margin);

    bool didWork = false;

    for(;j != last; ++i, ++j) if(*i > *j)
    {
      didWork = true;
      std::iter_swap(i, j);
    }

    if(!didWork && (margin == 1)) break;
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

  using value_type = unsigned;

  std::vector<value_type> values;

  for(value_type value = 0; inputStream >> value;)
  {
    values.emplace_back(value);

    switch(inputStream.peek()) case '\n': case EOF:
    {
      assert(!values.empty());

      std::cout << ::combSort(values.begin(), values.end()) << '\n';

      values.clear();
    }
  }
}
