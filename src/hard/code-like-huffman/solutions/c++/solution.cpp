#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

struct HuffmanCoding
{
  HuffmanCoding() : symbol() {}

  HuffmanCoding(const char symbol, const std::string& code) :
    symbol(symbol), code(code) {}

  bool operator<(const HuffmanCoding& other) const
  {
    return (this->symbol < other.symbol);
  }

  friend std::ostream& operator<<(std::ostream& outputStream,
                                  const HuffmanCoding& self)
  {
    return (outputStream << self.symbol << ": " << self.code << "; ");
  }

  char symbol;
  std::string code;
};

struct HuffmanNode
{
  HuffmanNode() : value() {}

  HuffmanNode(const std::string& name, const std::size_t value) :
    name(name), value(value) {}

  std::string name;
  std::size_t value;
  std::shared_ptr<HuffmanNode> lhs, rhs;
};

struct HuffmanNodeComparator
{
  bool operator()(const std::shared_ptr<HuffmanNode>& lhs,
                  const std::shared_ptr<HuffmanNode>& rhs) const
  {
    if(lhs->value != rhs->value) return (lhs->value > rhs->value);

    // Alpha ordering breaks the tie.
    return (lhs->name > rhs->name);
  }
};

template<typename MetricsContainer>
static std::set<HuffmanCoding> getHuffmanTable(const MetricsContainer& metrics)
{
  assert(!metrics.empty());

  using value_type = std::shared_ptr<HuffmanNode>;

  std::priority_queue<value_type, std::vector<value_type>,
    HuffmanNodeComparator> heap;

  // Seed heap.
  for(const auto& metric : metrics)
  {
    const auto name = std::string(1, metric.first);

    heap.emplace(std::make_shared<HuffmanNode>(name, metric.second));
  }

  // Build tree.
  while(heap.size() >= 2)
  {
    const auto lhs = heap.top();
    heap.pop();

    const auto rhs = heap.top();
    heap.pop();

    const auto parent = std::make_shared<HuffmanNode>();

    parent->lhs = lhs;
    parent->rhs = rhs;

    parent->value = (lhs->value + rhs->value);
    parent->name  = (' ' + lhs->name + rhs->name);

    heap.emplace(parent);
  }

  std::set<HuffmanCoding> results;

  // Build the mapping by walking the tree.
  const std::function<void(const value_type& node, std::string)> buildMapping =
    [&](const value_type& node, const std::string code)
  {
    if(!node) return;

    const auto isLeaf = (!node->lhs && !node->rhs);

    if(isLeaf)
    {
      assert(node->name.size() == 1);

      results.emplace(node->name.front(), code);
    }

    else
    {
      buildMapping(node->lhs, (code + '0'));
      buildMapping(node->rhs, (code + '1'));
    }
  };

  buildMapping(heap.top(), "");

  return results;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::unordered_map<char, std::size_t> metrics;

  for(std::string line; std::getline(inputStream, line); std::cout << '\n')
  {
    // As per the problem statement.
    assert((line.size() >= 3) && (line.size() <= 30));
    assert(std::all_of(line.cbegin(), line.cend(), ::islower));

    metrics.clear();

    // Collect letter frequencies.
    for(const auto c : line) ++metrics[c];

    // Serialize the Huffman coding table.
    for(const auto& entry : ::getHuffmanTable(metrics))
    {
      std::cout << entry;
    }
  }
}
