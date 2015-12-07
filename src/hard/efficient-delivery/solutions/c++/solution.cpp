#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <locale>
#include <sstream>
#include <type_traits>
#include <vector>

template<typename T>
static void dumpDeliveryOptions(const std::vector<T>& capacities,
                                const T targetVolume,
                                std::ostream& outputStream = std::cout)
{
  const auto tankerCount = capacities.size();

  // Tanker count.
  assert((tankerCount >= 2) && (tankerCount <= 5));

  // Tanker capacities.
  assert(std::all_of(capacities.cbegin(), capacities.cend(), [](const T value)
  {
    return ((value >= 2) && (value <= 1e4));
  }));

  // Supplied tanker capacities are guaranteed to be pre-sorted.
  assert(std::is_sorted(capacities.cbegin(), capacities.cend()));

  // Target oil volume.
  assert((targetVolume >= 1) && (targetVolume <= 2e5));

  std::stringstream outputBuffer;
  std::vector<std::size_t> tankers(tankerCount);

  using U = typename std::make_signed<T>::type;

  const std::function<void(std::size_t, U)> dump = [&](const std::size_t index,
                                                       const U volume)
  {
    assert(index <= tankerCount);

    // Last option, evenly divisible.
    if(index == (tankerCount - 1))
    {
      if(!(volume % capacities[index]))
      {
        tankers[index] = (volume / capacities[index]);

        dump((index + 1), 0);
      }
    }

    // Done.
    else if(index == tankerCount)
    {
      outputBuffer << '[' << tankers.front();

      for(auto i = decltype(tankerCount){1}; i < tankerCount; ++i)
      {
        outputBuffer << ',' << tankers[i];
      }

      outputBuffer << ']';
    }

    // Try next arrangements.
    else for(std::size_t i = 0; i <= (volume / capacities[index]); ++i)
    {
      tankers[index] = i;

      dump((index + 1), (volume - (i * capacities[index])));
    }
  };

  const auto canDeliver = [&](const U volume)
  {
    dump(0, volume);

    // Dumped some results to the output stream buffer.
    return (outputBuffer.tellp() > 0);
  };

  // Options exist as is.
  if(canDeliver(targetVolume)) outputStream << outputBuffer.rdbuf() << '\n';

  // Inflate target oil volume until delivery options become possible.
  else for(auto volume = targetVolume;;) if(canDeliver(++volume))
  {
    outputStream << (volume - targetVolume) << '\n';

    break;
  }
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Add parentheses and commas to the list of stream delimiters.
  inputStream.imbue(std::locale(inputStream.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    for(const auto c : "(,)") if(c) categories[c] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  std::vector<unsigned> values;

  for(decltype(values)::value_type value = 0; inputStream >> value;)
  {
    values.emplace_back(value);

    switch(inputStream.peek()) case '\n': case EOF:
    {
      const auto targetVolume = values.back();

      values.pop_back();

      ::dumpDeliveryOptions(values, targetVolume);

      values.clear();
    }
  }
}
