#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <locale>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

template<typename T> class Node
{
  public: using value_type = T;

  public: Node() : value() {}
  public: Node(const T& value) : value(value) {}

  public: void addChild(const Node* const child)
  {
    assert(child);

    this->children.insert(child);
  }

  private: friend std::ostream& operator<<(std::ostream& outputStream,
                                           const Node& node)
  {
    outputStream << node.value;

    bool isParent = true;

    for(const auto& child : node.children)
    {
      outputStream << (isParent ? " [" : ", ") << *child;

      isParent = false;
    }

    if(!isParent) outputStream << ']';

    return outputStream;
  }

  private: struct NodeComparator
  {
    bool operator()(const Node* const lhs, const Node* const rhs) const
    {
      return (lhs->value < rhs->value);
    }
  };

  private: T value;
  private: std::set<const Node*, NodeComparator> children;
};

static void dumpTree(const std::vector<std::string>& pairs,
                     std::ostream& outputStream = std::cout)
{
  assert(!pairs.empty());

  assert(std::all_of(pairs.cbegin(), pairs.cend(), [](const std::string& pair)
  {
    return (pair.size() == 2);
  }));

  using Node = Node<std::string::value_type>;

  std::unordered_map<Node::value_type, Node> nodes;

  const auto makeBranch = [&](const std::string& pair)
  {
    const auto lhs = pair[0], rhs = pair[1];

    const auto parent = [&]
    {
      auto parent = nodes.find(lhs);

      if(parent == nodes.cend()) parent = nodes.emplace(lhs, lhs).first;

      return &parent->second;
    }();

    const auto child = &nodes.emplace(rhs, rhs).first->second;

    parent->addChild(child);
  };

  // Grow.
  for(const auto& pair : pairs) makeBranch(pair);

  // Serialize the tree starting from the root node.
  outputStream << nodes[pairs.front().front()] << '\n';
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Add pipes to the list of stream delimiters.
  inputStream.imbue(std::locale(inputStream.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    categories['|'] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  std::vector<std::string> pairs;

  for(std::string chunk; inputStream >> chunk;)
  {
    pairs.emplace_back(chunk);

    switch(inputStream.peek()) case '\n': case EOF:
    {
      ::dumpTree(pairs);

      pairs.clear();
    }
  }
}
