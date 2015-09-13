#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

template<typename T> class SiteSurvey
{
  public: T getMstLength()
  {
    this->fillDistanceTable();

    const std::size_t tableWidth = this->distances.size();

    static constexpr auto maxTypeValue = std::numeric_limits<T>::max();

    std::vector<bool> used(tableWidth);
    std::vector<std::size_t> indices(tableWidth);
    std::vector<T> values(tableWidth, maxTypeValue);

    for(std::size_t i = 0; i < tableWidth; ++i)
    {
      // Find the next best unused node. The upper bound is pretty small so
      // dumb linear search should be fast enough.
      const auto j = [&]
      {
        std::size_t result = 0;
        T value = maxTypeValue;

        for(std::size_t k = 0; k < tableWidth; ++k) if(!used[k])
        {
          if(values[k] < value)
          {
            result = k;
            value  = values[k];
          }
        }

        used[result] = true;

        return result;
      }();

      // Record path.
      for(std::size_t k = 0; k < tableWidth; ++k) if(!used[k])
      {
        const auto& value = this->distances[j][k];

        if(value < values[k])
        {
          values[k]  = value;
          indices[k] = j;
        }
      }
    }

    // Tally path sum.
    return [&]
    {
      T result = 0;

      for(std::size_t i = 0; i < tableWidth; ++i)
      {
        result += this->distances[indices[i]][i];
      }

      return result;
    }();
  }

  private: void fillDistanceTable()
  {
    this->distances.clear();

    if(this->probes.empty()) return;

    const auto probeCount = this->probes.size();

    // Resize the distance table.
    {
      using row_type = typename decltype(this->distances)::value_type;

      this->distances.resize(probeCount, row_type(probeCount));
    }

    // Record probe distances relative to each other.
    for(std::size_t i = (0 + 0); i < probeCount; ++i)
    for(std::size_t j = (i + 1); j < probeCount; ++j)
    {
      const auto distance = this->probes[i].getDistance(this->probes[j]);

      this->distances[i][j] = distance;
      this->distances[j][i] = distance;
    }
  }

  private: friend std::istream& operator>>(std::istream& inputStream,
                                           SiteSurvey& site)
  {
    auto& probes = site.probes;

    probes.clear();

    // Fetch positions.
    for(typename decltype(site.probes)::value_type probe;
        inputStream >> probe;)
    {
      probes.emplace_back(probe);

      const auto peek = inputStream.peek();

      if(peek == '\n' || peek == EOF) break;
    }

    // Remove dupes.
    if(!probes.empty())
    {
      std::sort(probes.begin(), probes.end());

      probes.erase(std::unique(probes.begin(), probes.end()), probes.end());
    }

    return inputStream;
  }

  private: struct Probe
  {
    Probe() : x(), y() {}

    T getDistance(const Probe& other) const
    {
      return ::sqrt(::pow(other.x - this->x, 2) +
                    ::pow(other.y - this->y, 2));
    }

    bool operator<(const Probe& other) const
    {
      const auto x0 = this->x, y0 = this->y,
                 x1 = other.x, y1 = other.y;

      // A la std::pair.
      return ((x0 < x1) || (!(x1 < x0) && (y0 < y1)));
    }

    bool operator==(const Probe& other) const
    {
      return ((this->x == other.x) && (this->y == other.y));
    }

    friend std::istream& operator>>(std::istream& inputStream, Probe& probe)
    {
      static constexpr auto ignoreLimit =
        std::numeric_limits<std::streamsize>::max();

      inputStream >> probe.x;

      return inputStream.ignore(ignoreLimit, ',') >> probe.y;
    }

    T x, y;
  };

  private: std::vector<Probe> probes;
  private: std::vector<std::vector<T>> distances;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(SiteSurvey<float> site; inputStream >> site;)
  {
    std::cout << std::ceil(site.getMstLength()) << '\n';
  }
}
