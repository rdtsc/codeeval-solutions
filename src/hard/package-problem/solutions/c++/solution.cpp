#include <bitset>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <vector>

struct Item
{
  Item() : id(), value(), weight() {}

  friend std::istream& operator>>(std::istream& inputStream, Item& item)
  {
    const auto jumpTo = [&](const char c) -> decltype(inputStream)
    {
      assert(::isprint(c));

      static constexpr auto ignoreLimit =
        std::numeric_limits<std::streamsize>::max();

      return inputStream.ignore(ignoreLimit, c);
    };

    double weight = 0, value = 0;

    jumpTo('(') >> item.id;
    jumpTo(',') >> weight;
    jumpTo('$') >> value;
    jumpTo(')');

    // As per the problem statement.
    assert(item.id < 16);

    item.weight = (weight * 100);
    item.value  = (value  * 100);

    return inputStream;
  }

  unsigned id, value, weight;
};

struct Package
{
  Package() : items(), value(), weight() {}

  friend std::ostream& operator<<(std::ostream& outputStream,
                                  const Package& package)
  {
    // No item IDs.
    if(!package.items) return outputStream << '-';

    static constexpr std::size_t itemMaskWidth =
      std::numeric_limits<decltype(package.items)>::digits;

    std::bitset<itemMaskWidth> items(package.items);

    std::size_t i = 0;

    // Advance to first item ID.
    while(!items[i]) ++i;

    outputStream << (i + 1);

    // Dump any remaining item IDs.
    for(++i; i < itemMaskWidth; ++i) if(items[i])
    {
      outputStream << ',' << (i + 1);
    }

    return outputStream;
  }

  void addItem(const Item& item)
  {
    assert(item.id);

    this->items  |= (1 << (item.id - 1));
    this->value  += item.value;
    this->weight += item.weight;
  }

  std::uint_least32_t items;
  unsigned value, weight;
};

template<class Iterator>
static Package& doGetBestPackage(const Iterator item,
                                 const Iterator itemsEnd,
                                 const unsigned boxCapacity,
                                 Package& result)
{
  if(item == itemsEnd) return result;

  const auto nextItem = std::next(item);

  if(boxCapacity < item->weight)
  {
    return doGetBestPackage(nextItem, itemsEnd, boxCapacity, result);
  }

  Package take, pass;

  // Take the item.
  {
    const auto newBoxCapacity = (boxCapacity - item->weight);

    doGetBestPackage(nextItem, itemsEnd, newBoxCapacity, take);

    take.addItem(*item);
  }

  // Pass on the item.
  doGetBestPackage(nextItem, itemsEnd, boxCapacity, pass);

  // No difference in value.
  if(take.value == pass.value)
  {
    // Keep the item with the smaller weight.
    result = ((take.weight < pass.weight) ? take : pass);
  }

  // Keep the item with the larger value.
  else result = ((take.value > pass.value) ? take : pass);

  return result;
}

static Package getBestPackage(const std::vector<Item>& items,
                              const unsigned boxCapacity)
{
  assert(!items.empty() && boxCapacity);

  const auto itemsBegin = items.cbegin(),
             itemsEnd   = items.cend();

  Package result;

  return ::doGetBestPackage(itemsBegin, itemsEnd, boxCapacity, result);
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::vector<Item> items;

  for(unsigned boxCapacity = 0; inputStream >> boxCapacity;)
  {
    items.clear();

    boxCapacity *= 100;

    for(Item item; inputStream >> item;)
    {
      items.push_back(item);

      const auto peek = inputStream.peek();

      if(peek == '\n' || peek == EOF) break;
    }

    // As per the problem statement.
    assert(items.size() < 16);

    std::cout << ::getBestPackage(items, boxCapacity) << '\n';
  }
}
