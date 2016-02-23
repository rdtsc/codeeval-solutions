#include <algorithm>
#include <cassert>
#include <cstddef>
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <locale>
#include <vector>

enum class Action : unsigned char
{
  Enter, Leave, Unknown
};

struct Event
{
  Event() : Event(0, Action::Unknown, 0) {}

  Event(const Event& other, const std::size_t slot) :
    Event(other.id, other.action, slot) {}

  Event(const std::size_t id, const Action action, const std::size_t slot) :
    action(action), id(id), slot(slot) {}

  void set(const std::size_t slot)
  {
    this->slot = slot;
  }

  void set(const std::size_t slot, const Action action)
  {
    this->slot   = slot;
    this->action = action;
  }

  operator std::size_t() const
  {
    return this->id;
  }

  operator Action() const
  {
    return this->action;
  }

  friend std::istream& operator>>(std::istream& inputStream, Event& self)
  {
    char eventType = '\0';

    if(!(inputStream >> eventType >> self.id)) return inputStream;

    // As per the problem statement.
    assert(self.id <= 2e3);

    switch(eventType)
    {
      case 'E': self.action = Action::Enter; break;
      case 'L': self.action = Action::Leave; break;

      default : assert(false);
    }

    self.slot = 0;

    return inputStream;
  }

  Action action;
  std::size_t id, slot;
};

template<class Container, class Predicate>
static typename Container::const_iterator get(const Container& container,
                                              const Predicate predicate)
{
  return std::find_if(std::begin(container), std::end(container), predicate);
}

template<class Container, class Predicate>
static void pop(Container& container, const Predicate predicate)
{
  const auto newTail =
    std::remove_if(std::begin(container), std::end(container), predicate);

  container.erase(newTail, std::end(container));
}

template<class EventContainer>
static int getCriminalCount(const EventContainer& events)
{
  using value_type = typename EventContainer::value_type;

  std::deque<value_type> state;
  std::deque<std::size_t> ingress, egress;

  const auto eventCount = events.size();

  for(auto i = decltype(eventCount){}; i < eventCount; ++i)
  {
    const auto& entity = events[i];

    // Masked.
    if(!entity)
    {
      // Entering.
      if(entity == Action::Enter) ingress.emplace_back(i);

      // Leaving.
      else
      {
        const auto needle = ::get(state, [&](const value_type& entry)
        {
          return (entry == Action::Enter);
        });

        if(needle != state.cend())
        {
          egress.emplace_back(i);
        }

        else if(!ingress.empty()) ingress.pop_front();
      }

      continue;
    }

    const auto needle = std::find_if(state.begin(), state.end(),
      [&](const value_type& entry)
    {
      return (entry.id == entity);
    });

    // New.
    if(needle == state.end())
    {
      if((entity == Action::Leave) && !ingress.empty())
      {
        ingress.pop_front();
      }

      state.emplace_back(entity, i);
    }

    // Entering.
    else if(entity == Action::Enter)
    {
      if(*needle == Action::Leave) needle->set(i, entity.action);

      else
      {
        const auto slot = ::get(egress, [&](const std::size_t entry)
        {
          return (entry > needle->slot);
        });

        // No match.
        if(slot == egress.cend()) return -1;

        needle->set(i);

        ::pop(egress, [&](const std::size_t entry)
        {
          return (entry == *slot);
        });
      }
    }

    // Leaving.
    else
    {
      if(needle->action == Action::Enter) needle->set(i, entity.action);

      else
      {
        const auto slot = ::get(ingress, [&](const std::size_t entry)
        {
          return (entry > needle->slot);
        });

        if(slot == ingress.cend()) return -1;

        needle->set(i);

        ::pop(ingress, [&](const std::size_t entry)
        {
          return (entry == *slot);
        });
      }
    }
  }

  // Occupancy.
  ::pop(state, [](const value_type& entry)
  {
    return (entry != Action::Enter);
  });

  // Clean.
  for(const auto i : egress)
  {
    {
      const auto needle = ::get(state, [&](const value_type& entry)
      {
        return (entry.slot < i);
      });

      if(needle != state.cend())
      {
        ::pop(state, [&](const value_type& entry)
        {
          return (entry.slot == needle->slot);
        });

        continue;
      }
    }

    {
      const auto needle = ::get(ingress, [&](const std::size_t entry)
      {
        return (entry < i);
      });

      if(needle != ingress.cend())
      {
        ::pop(ingress, [&](const std::size_t entry)
        {
          return (entry == *needle);
        });
      }
    }
  }

  return (ingress.size() + state.size());
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Add semicolons and pipes to the list of stream delimiters.
  inputStream.imbue(std::locale(inputStream.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    for(const auto c : ";|") if(c) categories[c] |= facet::space;

    categories[';'] = categories['|'] = categories['|'] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  std::vector<Event> events;

  events.reserve(15);

  for(std::size_t n = 0; inputStream >> n; std::cout << '\n')
  {
    events.clear();

    for(auto i = n; i--;)
    {
      Event event;

      if(!(inputStream >> event)) assert(false);

      events.emplace_back(event);
    }

    assert(events.size() == n);

    const auto criminalCount = ::getCriminalCount(events);

    if(criminalCount < 0) std::cout << "CRIME TIME";

    else std::cout << criminalCount;
  }
}
