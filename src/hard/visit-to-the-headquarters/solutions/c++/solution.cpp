#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

struct Timestamp
{
  Timestamp() = default;
  Timestamp(const unsigned seconds) : value(seconds) {}

  operator unsigned() const
  {
    return this->value;
  }

  operator unsigned&()
  {
    return this->value;
  }

  friend std::istream& operator>>(std::istream& inputStream, Timestamp& self)
  {
    static constexpr auto ignoreLimit =
      std::numeric_limits<std::streamsize>::max();

    unsigned short h = 0, m = 0, s = 0;

    // ISO 8601/Extended format.
    inputStream >> h;
    inputStream.ignore(ignoreLimit, ':') >> m;
    inputStream.ignore(ignoreLimit, ':') >> s;

    assert((h <= 23) && (m <= 59) && (s <= 59));

    self.value = ((h * 3600) + (m * 60) + s);

    return inputStream;
  }

  friend std::ostream& operator<<(std::ostream& outputStream,
                                  const Timestamp& self)
  {
    #define CHUNK(Chunk) std::setw(2) << std::setfill('0') << (Chunk)

    const auto h = (self.value / 3600) % 24,
               m = (self.value /   60) % 60,
               s = (self.value %   60);

    return outputStream << CHUNK(h) << ':' << CHUNK(m) << ':' << CHUNK(s);

    #undef CHUNK
  }

  unsigned value = 0;
};

struct Stop
{
  friend std::istream& operator>>(std::istream& inputStream, Stop& self)
  {
    if(!(inputStream >> self.id >> self.duration)) return inputStream;

    // Given as XXYY, where XX is the floor number and YY is the room number.
    self.position = ((self.id / 100) * 100);

    return inputStream;
  }

  unsigned id = 0, position = 0, duration = 0;
};

struct Agent
{
  void setPosition(const unsigned position, const unsigned duration = 10)
  {
    this->egress += duration;
    this->position = position;
  }

  friend std::istream& operator>>(std::istream& inputStream, Agent& self)
  {
    self.itinerary.clear();

    // Fetch ID.
    {
      if(!(inputStream >> self.id)) return inputStream;

      assert(::isupper(self.id));
    }

    // Fetch ingress timestamp.
    {
      if(!(inputStream >> self.ingress)) return inputStream;

      self.egress = self.ingress;
    }

    // Fetch itinerary.
    {
      for(Stop stop; inputStream >> stop;)
      {
        self.itinerary.emplace_back(stop);

        switch(inputStream.peek()) case '\n': case EOF:
        {
          return inputStream;
        }
      }
    }

    return inputStream;
  }

  char id = '\0';
  unsigned position = 0;
  Timestamp ingress, egress;
  std::deque<Stop> itinerary;
};

template<class Iterator> struct AgentIteratorComparator
{
  bool operator()(const Iterator lhs, const Iterator rhs) const
  {
    if(lhs->egress != rhs->egress) return (lhs->egress > rhs->egress);

    // Alpha ordering breaks the tie.
    return (lhs->id > rhs->id);
  }
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Designates positions outside of the building.
  static constexpr unsigned outdoors = 0;

  // Designates the initial building injection position located in the
  // lobby (00) on the first floor of the building (01).
  static constexpr unsigned lobby = 100;

  std::vector<Agent> agents;

  agents.reserve(26);

  for(Agent agent; inputStream;)
  {
    inputStream >> agent;

    if(agent.itinerary.empty()) break;

    // Stop IDs appear to be pre-sorted.
    assert(std::is_sorted(agent.itinerary.cbegin(), agent.itinerary.cend(),
      [](const Stop& lhs, const Stop& rhs)
    {
      return (lhs.id < rhs.id);
    }));

    agent.setPosition(lobby);
    agents.emplace_back(agent);
  }

  using value_type = decltype(agents)::iterator;

  std::priority_queue<value_type, std::vector<value_type>,
    AgentIteratorComparator<value_type>> queue;

  // Inject agents into main queue.
  for(auto i = agents.begin(), j = agents.end(); i != j; ++i)
  {
    queue.emplace(i);
  }

  // Looks like the largest possible building consists of ten floors with ten
  // rooms per floor. This is small enough to justify the memory overhead vs
  // using a traditional map with a lookup penalty.
  static std::array<unsigned, 1011> rooms = {0};

  // Run simulation.
  while(!queue.empty())
  {
    const auto agent = queue.top();
    auto& position = agent->position;

    queue.pop();

    // Tick. TODO: Elevator queue.
    if(!agent->itinerary.empty())
    {
      const auto& stop = agent->itinerary.front();

      if(position == stop.position)
      {
        if(rooms[stop.id] > agent->egress) agent->egress = rooms[stop.id];

        else
        {
          rooms[stop.id] = (stop.duration + agent->egress);
          agent->egress += (stop.duration + 10);
          agent->itinerary.pop_front();
        }
      }

      else
      {
        const auto hi = ((stop.id / 100)) * 10,
                   lo = (position /  10);

        agent->egress += (10 + hi - lo);
        position = stop.position;
      }

      queue.emplace(agent);
    }

    // Done with our appointments.
    else
    {
      const unsigned floorDelta = (position / 100);

      agent->setPosition(outdoors, (floorDelta * 10));
    }
  }

  // Dump agent ingress/egress timestamps.
  for(const auto& agent : agents)
  {
    std::cout << agent.ingress << ' ' << agent.egress << '\n';
  }
}
