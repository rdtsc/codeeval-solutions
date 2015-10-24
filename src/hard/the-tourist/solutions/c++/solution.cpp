#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <sstream>
#include <vector>

// Reusing challenge 164's solution.
template<typename T> class Itinerary
{
  public: T getMstLength()
  {
    assert(!this->edges.empty());

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

        if(value && (value < values[k]))
        {
          values[k]  = value;
          indices[k] = j;
        }
      }
    }

    // Presence of unvisited vertices means the "must visit all cities"
    // constraint can't be satisfied.
    if(std::find(used.cbegin(), used.cend(), false) != used.cend()) return 0;

    T result = 0;

    // Tally path sum.
    for(std::size_t i = 0; i < tableWidth; ++i)
    {
      result += this->distances[indices[i]][i];
    }

    return result;
  }

  private: void fillDistanceTable()
  {
    this->distances.clear();

    if(this->edges.empty()) return;

    const auto maxVertexId = [&]
    {
      T result = 0;

      for(const auto& edge : this->edges)
      {
        const auto peak = std::max(edge.u, edge.v);

        if(peak > result) result = peak;
      }

      return result;
    }();

    // As per the problem statement.
    assert((maxVertexId >= 3) && (maxVertexId <= 100));

    // Resize the distance table.
    {
      using row_type = typename decltype(this->distances)::value_type;

      this->distances.resize(maxVertexId, row_type(maxVertexId));
    }

    for(const auto& edge : this->edges)
    {
      this->distances[edge.u - 1][edge.v - 1] = edge.w;
      this->distances[edge.v - 1][edge.u - 1] = edge.w;
    }
  }

  private: friend std::istream& operator>>(std::istream& inputStream,
                                           Itinerary& itinerary)
  {
    itinerary.edges.clear();

    std::string line;

    if(!std::getline(inputStream, line)) return inputStream;

    // Edges are delimited by pipes.
    std::replace(line.begin(), line.end(), '|', ' ');

    std::istringstream tokenizer(line);

    using input_iterator = std::istream_iterator<Edge>;

    std::copy(input_iterator(tokenizer),
              input_iterator(),
              std::back_inserter(itinerary.edges));

    return inputStream;
  }

  private: struct Edge
  {
    Edge() : u(), v(), w() {}

    friend std::istream& operator>>(std::istream& inputStream, Edge& edge)
    {
      return inputStream >> edge.u  // Vertex A.
                         >> edge.v  // Vertex B.
                         >> edge.w; // Edge weight.
    }

    T u, v, w;
  };

  private: std::vector<Edge> edges;
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

  for(Itinerary<unsigned> itinerary; inputStream >> itinerary;)
  {
    const auto pathLength = itinerary.getMstLength();

    if(!pathLength) std::cout << "False\n";

    else std::cout << pathLength << '\n';
  }
}
