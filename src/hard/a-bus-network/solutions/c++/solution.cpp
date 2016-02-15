#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

template<typename T> class Itinerary
{
  public: Itinerary() : sourceId(), targetId(), nodeCount()
  {
    static_assert(std::is_unsigned<T>::value,
      "Expecting an unsigned type.");

    this->routes.reserve(150);
  }

  public: T getTripTime()
  {
    assert(this->nodeCount && this->search.empty());

    this->distances.clear();
    this->distances.resize(this->nodeCount, this->unreachable);

    this->used.clear();
    this->used.resize(this->nodeCount, false);

    // Seed starting points.
    for(const auto id : this->nodes[this->sourceId])
    {
      this->distances[id] = 0;
      this->search.emplace(id);
    }

    // Get distances.
    while(!this->search.empty())
    {
      const auto id = this->search.front();
      this->search.pop();

      this->used[id] = true;

      for(const auto& edge : this->edges[id]) if(!used[edge.target])
      {
        this->search.emplace(edge.target);

        const auto newDistance = (this->distances[id] + edge.weight);

        if(this->distances[edge.target] > newDistance)
        {
          this->distances[edge.target] = newDistance;
        }
      }
    }

    // Shortest route length.
    return [&]
    {
      auto result = this->unreachable;

      for(const auto id : this->nodes[this->targetId])
      {
        if(this->distances[id] < result)
        {
          result = this->distances[id];
        }
      }

      return result;
    }();
  }

  private: void buildNetwork(const T continuePenalty = 7,
                             const T transferPenalty = 12)
  {
    assert(this->nodeCount);

    /*
                                         C = 7'    T = 12'
                                       ┌───┐   ┌───┐   ┌───┐
                                       │ 1 ├─C─┤ 2 ├─C─┤ 3 ├─┐
                                       └───┘   └───┘   └─┬─┘ │
                                         ┌───────T───────┘   │
                  R1 = [1, 2, 3]       ┌─┴─┐   ┌───┐   ┌───┐ │
                  R2 = [3, 4, 5]   →   │ 3 ├─C─┤ 4 ├─C─┤ 5 │ T
                  R3 = [6, 7, 3]       └─┬─┘   └───┘   └───┘ │
                                         └───────T───────┐   │
                                       ┌───┐   ┌───┐   ┌─┴─┐ │
                                       │ 6 ├─C─┤ 7 ├─C─┤ 3 ├─┘
                                       └───┘   └───┘   └───┘
    */

    this->edges.clear();

    // Lay intra-route edges.
    {
      std::size_t baseId = 0;

      for(const auto& route : this->routes)
      {
        const auto stopCount = route.stops.size();

        for(std::size_t i = 0; i < (stopCount - 1); ++i)
        {
          const auto u = (i + baseId),
                     v = (u + 1);

          this->edges[u].emplace_back(v, continuePenalty);
          this->edges[v].emplace_back(u, continuePenalty);
        }

        baseId += stopCount;
      }
    }

    // Lay inter-route edges.
    for(const auto& entry : this->nodes)
    {
      const auto& aliases = entry.second;
      const auto aliasCount = aliases.size();

      for(std::size_t i = (  0  ); i < aliasCount; ++i)
      for(std::size_t j = (i + 1); j < aliasCount; ++j)
      {
        const auto u = aliases[i],
                   v = aliases[j];

        this->edges[u].emplace_back(v, transferPenalty);
        this->edges[v].emplace_back(u, transferPenalty);
      }
    }
  }

  private: friend std::istream& operator>>(std::istream& inputStream,
                                           Itinerary& self)
  {
    std::string line;

    if(!std::getline(inputStream, line)) return inputStream;

    self.nodeCount = 0;
    self.nodes.clear();
    self.routes.clear();

    // Naked pointers are a little faster than iterators here.
    auto cursor = line.c_str();

    const auto getNextValue = [&]
    {
      T result = 0;

      #define IS_PAYLOAD() (*cursor >= '0' && *cursor <= '9')

      // Eat anything that's not a digit.
      while(*cursor && !IS_PAYLOAD()) ++cursor;

      for(;*cursor; ++cursor)
      {
        if(!IS_PAYLOAD()) break;

        result = (result * 10) + (*cursor - '0');
      }

      #undef IS_PAYLOAD

      return result;
    };

    // (1,2); R1=[1,2,3]
    //  ^ ^
    self.sourceId = getNextValue();
    self.targetId = getNextValue();

    for(Route route; *cursor; route.reset())
    {
      // Advance to next route declaration.
      while(*cursor && (*cursor != 'R')) ++cursor;

      if(!*cursor) break;

      // (1,2); R1=[1,2,3]
      //         ^
      route.name = getNextValue();

      // (1,2); R1=[1,2,3]
      //            ^ ...
      while(*cursor && (*cursor != ']'))
      {
        const auto stopId = getNextValue();

        route.stops.emplace_back(stopId);
        self.nodes[stopId].emplace_back(self.nodeCount++);
      }

      self.routes.emplace_back(route);
    }

    self.buildNetwork();

    return inputStream;
  }

  private: struct Route
  {
    Route() : name()
    {
      this->stops.reserve(35);
    }

    void reset()
    {
      this->name = T();
      this->stops.clear();
    }

    T name;
    std::vector<T> stops;
  };

  private: struct Edge
  {
    Edge(const T target, const T weight) :
      target(target), weight(weight) {}

    T target, weight;
  };

  private: T sourceId, targetId;
  private: std::size_t nodeCount;

  private: std::vector<Route> routes;
  private: std::unordered_map<T, std::vector<T>> nodes;
  private: std::unordered_map<T, std::vector<Edge>> edges;

  private: std::queue<T> search;
  private: std::vector<bool> used;
  private: std::vector<T> distances;

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

  for(Itinerary<unsigned short> itinerary; inputStream >> itinerary;)
  {
    const auto tripTime = itinerary.getTripTime();

    if(tripTime == itinerary.unreachable)
    {
      std::cout << "None\n";
    }

    else std::cout << tripTime << '\n';
  }
}
