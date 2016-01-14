#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <locale>
#include <queue>
#include <sstream>
#include <vector>

class Network
{
  public: explicit Network(std::istream& inputStream)
  {
    if(!(inputStream >> *this)) assert(false);
  }

  public: void dumpPaths(const unsigned sourceId,
                         const unsigned targetId,
                         std::ostream& outputStream = std::cout) const
  {
    using Path = Path<10>;

    std::queue<Path> queue;
    std::vector<Path> paths;

    auto bestPathLength = std::numeric_limits<std::size_t>::max();

    for(queue.emplace(sourceId); !queue.empty();)
    {
      const auto path = queue.front();
      const auto pathLength = path.size();

      queue.pop();

      // TODO: Fix hack.
      if(pathLength >= Path::maxLength) break;

      const auto thisId = path.back();

      if(thisId == targetId)
      {
        if(pathLength <= bestPathLength)
        {
          paths.emplace_back(path);
          bestPathLength = pathLength;
        }

        else break;
      }

      for(const auto& nextId : this->connections[thisId])
      {
        if(!path.contains(nextId))
        {
          auto newPath = path;
          newPath.append(nextId);
          queue.emplace(newPath);
        }
      }
    }

    if(paths.empty())
    {
      outputStream << "No connection\n";
      return;
    }

    std::sort(paths.begin(), paths.end());

    const auto pathsBegin = paths.cbegin(),
               pathsEnd   = paths.cend();

    outputStream << *pathsBegin;

    for(auto i = std::next(pathsBegin); i != pathsEnd; ++i)
    {
      outputStream << ", " << *i;
    }

    outputStream << '\n';
  }

  private: void makeConnections()
  {
    assert(!this->hosts.empty());

    this->connections.resize(this->hosts.size());

    const auto hostsBegin = this->hosts.cbegin(),
               hostsEnd   = this->hosts.cend();

    for(auto i = hostsBegin;   i != hostsEnd; ++i)
    for(auto j = std::next(i); j != hostsEnd; ++j) [&]
    {
      for(auto lhsInterface : i->interfaces)
      for(auto rhsInterface : j->interfaces)
      {
        if(lhsInterface == rhsInterface)
        {
          this->connections[i->id].emplace_back(j->id);
          this->connections[j->id].emplace_back(i->id);
          return;
        }
      }
    }();
  }

  private: friend std::istream& operator>>(std::istream& inputStream,
                                           Network& self)
  {
    std::string chunk;

    if(!std::getline(inputStream, chunk)) return inputStream;

    assert((chunk.front() == '{') && (chunk.back() == '}'));

    // Remove enclosing braces.
    chunk.erase(0, 1);
    chunk.pop_back();

    std::istringstream tokenizer(chunk), extractor;

    // Add the following characters to the list of stream delimiters: "[':,]"
    extractor.imbue(std::locale(extractor.getloc(), []
    {
      using facet = std::ctype<char>;

      const auto classicTableStart = facet::classic_table(),
                 classicTableEnd   = classicTableStart + facet::table_size;

      static std::vector<facet::mask> categories(classicTableStart,
                                                 classicTableEnd);

      for(const auto c : "[':,]") if(c) categories[c] |= facet::space;

      // Ref-counted upstream.
      return new facet(categories.data());
    }()));

    while(std::getline(tokenizer, chunk, ']'))
    {
      extractor.clear();
      extractor.str(chunk);

      Host host;

      if(!(extractor >> host.id)) assert(false);

      // Host has hot interfaces.
      else if(chunk.back() != '[') while(extractor >> chunk)
      {
        host.addInterface(chunk);
      }

      self.hosts.emplace_back(host);
    }

    self.makeConnections();

    return inputStream;
  }

  private: struct Host
  {
    Host() : id() {}

    // NOTE: Exact-width types are not guaranteed to be available on all
    // platforms.
    using ipv4_t = std::uint32_t;

    void addInterface(const std::string& cidr)
    {
      assert(std::all_of(cidr.cbegin(), cidr.cend(), [](const char c)
      {
        return (::isdigit(c) || (c == '.') || (c == '/'));
      }));

      assert(std::count(cidr.cbegin(), cidr.cend(), '.') == 3);
      assert(std::count(cidr.cbegin(), cidr.cend(), '/') == 1);

      char* cursor = nullptr;
      const char* address = cidr.c_str();

      const auto ip = [&]
      {
        ipv4_t result = 0;

        for(unsigned char shift = 4; shift--; address = ++cursor)
        {
          const auto octet = std::strtoul(address, &cursor, 10);

          assert((octet < (1 << 8)) && ((*cursor == '.') || (*cursor == '/')));

          result += ((1 << (8 * shift)) * octet);
        }

        return result;
      }();

      const auto mask = [&]
      {
        static const auto base = std::numeric_limits<ipv4_t>::digits;

        const auto offset = std::strtoul(address, &cursor, 10);

        return ~((ipv4_t{1} << (base - offset)) - 1);
      }();

      this->interfaces.emplace_back(ip & mask);
    }

    unsigned id;
    std::vector<ipv4_t> interfaces;
  };

  private: template<std::size_t MaxLength> struct Path
  {
    Path() : nodeCount() {}
    Path(const unsigned head) : nodes{head}, nodeCount(1) {}

    bool contains(const unsigned value) const
    {
      // TODO: Even though paths are short, this could be improved.
      for(std::size_t i = 0; i < this->nodeCount; ++i)
      {
        if(this->nodes[i] == value) return true;
      }

      return false;
    }

    void append(const unsigned value)
    {
      assert(this->nodeCount < this->nodes.size());

      this->nodes[this->nodeCount++] = value;
    }

    unsigned back() const
    {
      assert(this->nodeCount);

      return this->nodes[this->nodeCount - 1];
    }

    std::size_t size() const
    {
      return this->nodeCount;
    }

    bool operator<(const Path& other) const
    {
      const auto beginA = this->nodes.cbegin(),
                 beginB = other.nodes.cbegin();

      const auto endA = std::next(beginA, this->nodeCount),
                 endB = std::next(beginB, other.nodeCount);

      return std::lexicographical_compare(beginA, endA, beginB, endB);
    }

    friend std::ostream& operator<<(std::ostream& outputStream,
                                    const Path& self)
    {
      outputStream << '[' << self.nodes.front();

      for(std::size_t i = 1; i < self.nodeCount; ++i)
      {
        outputStream << ", " << self.nodes[i];
      }

      return outputStream << ']';
    }

    static const auto maxLength = MaxLength;
    std::array<unsigned, MaxLength> nodes;
    std::size_t nodeCount;
  };

  private: std::vector<Host> hosts;
  private: std::vector<std::vector<unsigned>> connections;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  Network network(inputStream);

  for(unsigned u = 0, v = 0; inputStream >> u >> v; network.dumpPaths(u, v));
}
