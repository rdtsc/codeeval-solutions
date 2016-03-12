#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

template<class Container>
static std::size_t partition(Container& values,
                             const std::ptrdiff_t head,
                             const std::ptrdiff_t tail)
{
  std::size_t result = head;

  for(auto lhs = head, rhs = tail;;)
  {
    // Right.
    while((result != rhs) && (values[result] < values[rhs])) --rhs;

    if(result == rhs) break;

    else if(values[result] > values[rhs])
    {
      std::swap(values[result], values[rhs]);
      result = rhs;
    }

    // Left.
    while((result != lhs) && (values[result] > values[lhs])) ++lhs;

    if(result == lhs) break;

    else if(values[result] < values[lhs])
    {
      std::swap(values[result], values[lhs]);
      result = lhs;
    }
  }

  return result;
}

template<class Container>
static std::size_t doQuickSort(Container& values,
                               const std::ptrdiff_t head,
                               const std::ptrdiff_t tail,
                               std::size_t i = 0)
{
  if(head >= tail) return i;

  const auto pivot = ::partition(values, head, tail);

  return ::doQuickSort(values, head, (pivot - 1), ++i) +
         ::doQuickSort(values, (pivot + 1), tail);
}

template<class Container> static std::size_t quickSort(Container& values)
{
  return ::doQuickSort(values, 0, (values.size() - 1));
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::vector<int> values;

  for(decltype(values)::value_type value = 0; inputStream >> value;)
  {
    values.emplace_back(value);

    switch(inputStream.peek()) case '\n': case EOF:
    {
      std::cout << ::quickSort(values) << '\n';

      values.clear();
    }
  }
}
