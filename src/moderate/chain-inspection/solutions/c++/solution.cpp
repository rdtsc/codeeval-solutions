#include <cassert>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>
#include <unordered_map>
#include <vector>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  using value_type = unsigned;

  std::unordered_map<value_type, value_type> chain;
  std::istringstream tokenizer;

  // Add dashes and semicolons to the list of stream delimiters.
  tokenizer.imbue(std::locale(inputStream.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    categories['-'] = categories[';'] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  for(std::string line; std::getline(inputStream, line);)
  {
    chain.clear();
    tokenizer.clear();
    tokenizer.str(line);

    while(tokenizer)
    {
      // Chain links look like "lhs-rhs" where the chunks match: (\d+|BEGIN|END)
      auto getLinkMember = [&]
      {
        value_type index = 0;
        const auto peek = line.front();

        // Chunk is "BEGIN" or "END".
        if(peek == 'B' || peek == 'E') index = (peek == 'E');

        // Chunk is an index.
        else index = std::stoi(line);

        return index;
      };

      if(!(tokenizer >> line)) break;
      const auto head = getLinkMember();

      if(!(tokenizer >> line)) break;
      const auto tail = getLinkMember();

      chain[head] = tail;
    }

    assert(!chain.empty());

    const auto linkCount = chain.size();

    // Visit each chain link, looking for repeats.
    for(value_type visitedLinkCount = 0, chainIndex = 0;
        visitedLinkCount <= linkCount; ++visitedLinkCount)
    {
      const auto oldIndex = chainIndex;

      // We've either been here before, or we're at the terminal link.
      if(!chain[chainIndex])
      {
        const bool isBad = (chainIndex != 1 || visitedLinkCount != linkCount);

        std::cout << (isBad ? "BAD" : "GOOD") << '\n';

        break;
      }

      chainIndex = chain[chainIndex];

      // Mark this link as visited.
      chain[oldIndex] = 0;
    }
  }
}
