#include <cassert>
#include <fstream>
#include <iostream>
#include <locale>
#include <stdexcept>
#include <string>
#include <vector>

static char wordToDigit(const std::string& word)
{
  assert(word.size() >= 3);

  switch(word[0])
  {
    case 'z': return 0; // [z]ero
    case 'o': return 1; // [o]ne
    case 'e': return 8; // [e]ight
    case 'n': return 9; // [n]ine

    default: switch(word[1])
    {
      case 'w': return 2; // t[w]o
      case 'h': return 3; // t[h]ree
      case 'o': return 4; // f[o]ur
      case 'i': return (word[2] == 'x') + 5; // f[i]ve || s[i]x
      case 'e': return 7; // s[e]ven
    }
  }

  throw std::out_of_range("Unknown literal.");
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Add semicolons to the list of stream delimiters.
  inputStream.imbue(std::locale(inputStream.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    categories[';'] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  for(std::string chunk; inputStream >> chunk;)
  {
    std::cout << static_cast<char>('0' + ::wordToDigit(chunk));

    const auto peek = inputStream.peek();

    if(peek == '\n' || peek == EOF) std::cout << '\n';
  }
}
