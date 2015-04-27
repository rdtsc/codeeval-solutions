#include <cassert>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>
#include <vector>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::vector<unsigned> numbers;
  std::istringstream tokenizer;

  // Add commas and hyphens to the list of stream delimiters.
  tokenizer.imbue(std::locale(inputStream.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    categories[','] = categories['-'] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  const auto resetTokenizer = [&](const std::string& contents)
  {
    tokenizer.clear();
    tokenizer.str(contents);
  };

  for(std::string line; std::getline(inputStream, line); std::cout << '\n')
  {
    // Get the number sequence before the colon.
    {
      numbers.clear();
      resetTokenizer(line);

      for(decltype(numbers)::value_type n = 0; tokenizer >> n;)
      {
        numbers.push_back(n);
      }
    }

    // Get target indices after the colon and apply swapping operations.
    {
      resetTokenizer(line);
      std::getline(tokenizer, line, ':');
      std::getline(tokenizer, line, ':');
      resetTokenizer(line);

      for(std::size_t lhs = 0, rhs = 0; tokenizer >> lhs >> rhs;)
      {
        std::swap(numbers[lhs], numbers[rhs]);
      }
    }

    for(const auto n : numbers) std::cout << n << ' ';
  }
}
