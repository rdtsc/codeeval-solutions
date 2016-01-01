#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <sstream>
#include <utility>
#include <vector>

template<typename T> class Itinerary
{
  public: Itinerary() : sourceId(), targetId() {}

  public: T getLength() const
  {
    assert(!this->edges.empty());

    const auto matrixWidth = this->matrix.size();

    std::vector<T> distances(matrixWidth, this->unreachable);
    std::vector<bool> used(matrixWidth);

    const auto forEachUnused = [&](const std::function<void(std::size_t)>& fn)
    {
      for(auto i = decltype(matrixWidth){}; i < matrixWidth; ++i)
      {
        if(!used[i]) fn(i);
      }
    };

    distances[this->sourceId - 1] = 0;

    // Dijkstra's algorithm.
    std::for_each(distances.cbegin(), distances.cend(), [&](T)
    {
      const auto i = [&]
      {
        std::pair<T, std::size_t> result(this->unreachable, 0);

        forEachUnused([&](const std::size_t i)
        {
          if(distances[i] <= result.first) result = {distances[i], i};
        });

        used[result.second] = true;

        return result.second;
      }();

      forEachUnused([&](const std::size_t j)
      {
        if(this->matrix[i][j] && (distances[i] != this->unreachable))
        {
          const auto distance = (this->matrix[i][j] + distances[i]);

          if(distance < distances[j]) distances[j] = distance;
        }
      });
    });

    return distances[this->targetId - 1];
  }

  private: void fillAdjacencyMatrix()
  {
    this->matrix.clear();

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

    assert(maxVertexId <= 1e3);

    // Resize the adjacency matrix.
    {
      using row_type = typename decltype(this->matrix)::value_type;

      this->matrix.resize(maxVertexId, row_type(maxVertexId));
    }

    for(const auto& edge : this->edges)
    {
      this->matrix[edge.u - 1][edge.v - 1] = edge.w;
      this->matrix[edge.v - 1][edge.u - 1] = edge.w;
    }
  }

  private: friend std::istream& operator>>(std::istream& inputStream,
                                           Itinerary& self)
  {
    self.edges.clear();

    std::string line;

    if(!std::getline(inputStream, line)) return inputStream;

    const auto delimiterOffset = line.find_last_of('|');

    // The source/destination edge pair is preceded by a pipe.
    assert(delimiterOffset != std::string::npos);

    // Edges are delimited by commas.
    std::replace(line.begin(), line.end(), ',', ' ');

    // Unserialize graph edges.
    std::istringstream tokenizer(line.substr(0, delimiterOffset));

    using input_iterator = std::istream_iterator<Edge>;

    std::copy(input_iterator(tokenizer),
              input_iterator(),
              std::back_inserter(self.edges));

    // Unserialize source/destination edge pair.
    tokenizer.clear();
    tokenizer.str(line.substr(delimiterOffset + 1));

    if(!(tokenizer >> self.sourceId >> self.targetId)) assert(false);

    assert((self.sourceId >= 1) && (self.sourceId != self.targetId));

    self.fillAdjacencyMatrix();

    return inputStream;
  }

  private: struct Edge
  {
    Edge() : u(), v(), w() {}

    friend std::istream& operator>>(std::istream& inputStream, Edge& self)
    {
      return inputStream >> self.u  // Vertex A.
                         >> self.v  // Vertex B.
                         >> self.w; // Edge weight.
    }

    T u, v, w;
  };

  private: std::vector<Edge> edges;
  private: std::vector<std::vector<T>> matrix;
  private: T sourceId, targetId;

  public: static constexpr T unreachable = std::numeric_limits<T>::max();
};

template<typename T> constexpr T Itinerary<T>::unreachable;

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
    const auto pathLength  = itinerary.getLength();

    if(pathLength == decltype(itinerary)::unreachable)
    {
      std::cout << "False\n";
    }

    else std::cout << pathLength << '\n';
  }
}
