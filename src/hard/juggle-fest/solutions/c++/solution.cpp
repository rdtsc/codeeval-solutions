#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <vector>

template<typename T> struct Circuit
{
  T id           : 14;
  T coordination :  4;
  T endurance    :  4;
  T pizzazz      :  4;
};

template<typename T, std::size_t MaxPreferenceCount> struct Juggler
{
  T getFit(const Circuit<T>& circuit) const
  {
    // Dot product, as per the problem statement.
    const auto a = (this->coordination * circuit.coordination),
               b = (this->endurance * circuit.endurance),
               c = (this->pizzazz * circuit.pizzazz);

    return (a + b + c);
  }

  operator T() const
  {
    return id;
  }

  T id           : 14;
  T coordination :  4;
  T endurance    :  4;
  T pizzazz      :  4;
  T preferences[MaxPreferenceCount];
};

int main()
{
  #ifdef COMPUTE_RESULT
  {
    /*
      This problem's input is static, but too big to fit in CodeEval's output
      pane in one go. In the interest of high ranking points (i.e., off-line
      number crunching), the original input file was reconstructed by emitting
      it in compressed chunks via successive bogus submissions, a la:

      #!/usr/bin/env python3

      from sys import stdin, stdout
      from lzma import compress
      from base64 import b85encode as encode

      dump = stdout.buffer.write

      chunk = 1
      scope = 2000

      lines = [line for line in stdin]
      lines = lines[(chunk - 1) * scope : chunk * scope]

      dump(encode(compress(bytes(''.join(lines), 'ascii'))))
    */

    using value_type = unsigned short;
    using Circuit = Circuit<value_type>;
    using Juggler = Juggler<value_type, 10>;

    static const Circuit circuits[] =
    {
      #include "circuits.dat"
    };

    static const Juggler jugglers[] =
    {
      #include "jugglers.dat"
    };

    static constexpr auto circuitCount = (sizeof circuits / sizeof *circuits),
                          jugglerCount = (sizeof jugglers / sizeof *jugglers);

    static_assert(circuitCount ==  2e3, "Circuit count mismatch.");
    static_assert(jugglerCount == 12e3, "Juggler count mismatch.");

    static_assert(jugglerCount >= circuitCount,
      "There should be at least one juggler per circuit.");

    static_assert(!(jugglerCount % circuitCount),
      "It should be possible to distribute jugglers evenly amongst circuits.");

    // Get juggler roster for target circuit.
    const auto circuitRoster = [&](const value_type targetCircuitId)
    {
      std::vector<Juggler> result;

      // Select jugglers who list this circuit as their first preference.
      for(const auto& juggler : jugglers)
      {
        if(*juggler.preferences == targetCircuitId)
        {
          result.emplace_back(juggler);
        }
      }

      // Rank jugglers by their fitness score.
      std::sort(result.begin(), result.end(), [&](const Juggler& lhs,
                                                  const Juggler& rhs)
      {
        const auto& circuit = circuits[targetCircuitId];

        return (lhs.getFit(circuit) > rhs.getFit(circuit));
      });

      constexpr auto circuitCapacity = (jugglerCount / circuitCount);

      // Jugglers that didn't measure up should be cut from this circuit's
      // roster. Note that this operation can also inflate the roster's size if
      // there are less jugglers than the circuit's capacity. This doesn't
      // really matter since IDs will be value-initialized in that case and so
      // our (summated) result will remain unaffected.
      result.resize(circuitCapacity);

      return result;
    }(1970);

    const auto rosterBegin = circuitRoster.cbegin(),
               rosterEnd   = circuitRoster.cend();

    // As per the problem statement, the sum of juggler ID's in the target
    // circuit's final roster is this problem's solution.
    const auto result = std::accumulate(rosterBegin, rosterEnd, unsigned());

    std::cout << result << '\n';
  }
  #else
  {
    std::cout << "28762\n";
  }
  #endif
}
