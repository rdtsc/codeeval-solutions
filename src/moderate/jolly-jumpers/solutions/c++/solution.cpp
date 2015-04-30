#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  using value_type = int;

  std::vector<value_type> terms;

  terms.reserve(128);

  const auto isJollyJumper = [](const std::vector<value_type>& terms)
  {
    assert(!terms.empty());

    const auto termCount = terms.size();

    if(termCount == 1) return true;

    static std::vector<bool> history;

    history.clear();
    history.resize(termCount);

    const auto termsEnd = std::prev(terms.cend());

    for(auto i = terms.cbegin(); i != termsEnd; ++i)
    {
      // Absolute difference between neighboring terms.
      const auto delta = std::abs(*i - *std::next(i));

      // The differences between successive elements should take on all
      // possible values in: [1, n).
      const bool isOutOfBounds = (!delta || (std::size_t)delta >= termCount);

      if(isOutOfBounds || history[delta - 1]) return false;

      history[delta - 1] = true;
    }

    return true;
  };

  for(std::size_t termCount = 0; inputStream >> termCount;)
  {
    terms.clear();

    using input_iterator =
      std::istream_iterator<decltype(terms)::value_type>;

    std::copy_n(input_iterator(inputStream), termCount,
      std::back_inserter(terms));

    std::cout << (isJollyJumper(terms) ? "J" : "Not j") << "olly\n";
  }
}
