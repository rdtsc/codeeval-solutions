#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <locale>
#include <string>
#include <vector>

class CodeEvalFacet : public std::numpunct<char>
{
  protected: virtual std::string do_truename() const override
  {
    return "True";
  }

  protected: virtual std::string do_falsename() const override
  {
    return "False";
  }
};

template<class BlockIterator, class WordIterator>
static bool tryArrangement(const BlockIterator blocksBegin,
                           const BlockIterator blocksEnd,
                           const WordIterator wordBegin,
                           const WordIterator wordEnd)
{
  assert((blocksEnd >= blocksBegin) && (wordEnd >= wordBegin));

  // Found.
  if(wordBegin == wordEnd) return true;

  // Exhausted.
  else if(blocksBegin == blocksEnd) return false;

  // Try next.
  for(auto i = blocksBegin; i != blocksEnd; ++i)
  {
    #define BIT_IS_SET_AT(Position) (*i & (1 << (Position)))

    const bool blockContainsLetter = BIT_IS_SET_AT(*wordBegin - 'A');

    #undef BIT_IS_SET_AT

    if(blockContainsLetter)
    {
      std::swap(*i, *blocksBegin);

      const auto nextBlock  = std::next(blocksBegin);
      const auto nextLetter = std::next(wordBegin);

      const bool isFound =
        tryArrangement(nextBlock, blocksEnd, nextLetter, wordEnd);

      std::swap(*i, *blocksBegin);

      if(isFound) return true;
    }
  }

  return false;
}

template<typename T> static bool canArrange(std::vector<T>& blocks,
                                            const std::string& word)
{
  static_assert(std::numeric_limits<T>::digits >= 26,
    "Type must be wide enough to accommodate the English alphabet.");

  assert(!word.empty() && (blocks.size() >= word.size()));

  const auto blocksBegin = blocks.begin(),
             blocksEnd   = blocks.end();

  const auto wordBegin = word.cbegin(),
             wordEnd   = word.cend();

  return ::tryArrangement(blocksBegin, blocksEnd, wordBegin, wordEnd);
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Add vertical pipes to the list of stream delimiters.
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

  // Set bools to be emitted as their textual equivalents in English.
  std::cout.imbue(std::locale(std::cout.getloc(), [&]
  {
    std::boolalpha(std::cout);

    return new CodeEvalFacet();
  }()));

  const auto isUpperAlpha = [](const std::string& string)
  {
    return std::all_of(string.cbegin(), string.cend(), [](const char c)
    {
      return (c >= 'A' && c <= 'Z');
    });
  };

  using block_type = std::uint_least32_t;

  std::vector<block_type> blocks;

  for(std::string word, block; inputStream;)
  {
    std::size_t blockCount = 0;

    if(!(inputStream >> blockCount >> word)) break;

    // As per the problem statement.
    assert((word.size() >= 3) && (word.size() <= 18) && isUpperAlpha(word));

    for(blocks.clear(), blocks.reserve(blockCount); inputStream >> block;)
    {
      // Blocks are cubic.
      assert((block.size() == 6) && isUpperAlpha(block));

      // Snapshot the block's face values.
      blocks.push_back([&]
      {
        block_type result = 0;

        #define SET_BIT_AT(Position) (result |= (1 << (Position)))

        for(const char c : block) SET_BIT_AT(c - 'A');

        #undef SET_BIT_AT

        return result;
      }());

      const auto peek = inputStream.peek();

      if(peek == '\n' || peek == EOF) break;
    }

    // As per the problem statement.
    assert(blocks.size() >= word.size());

    std::cout << ::canArrange(blocks, word) << '\n';
  }
}
