#include <cassert>
#include <fstream>
#include <iostream>
#include <locale>
#include <string>
#include <vector>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Add commas to the list of stream delimiters.
  inputStream.imbue(std::locale(inputStream.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    categories[','] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  std::vector<std::string> words, numbers;

  auto dump = [](const std::vector<std::string>& collection)
  {
    std::cout << collection.front();

    const auto elementCount = collection.size();

    for(auto i = decltype(elementCount){1}; i < elementCount; ++i)
    {
      std::cout << ',' << collection[i];
    }
  };

  for(std::string chunk; inputStream >> chunk;)
  {
    const char sniff = chunk.front();

    if(::isdigit(sniff)) numbers.push_back(chunk);

    else words.push_back(chunk);

    switch(inputStream.peek()) case '\n': case EOF:
    {
      const bool hasWords   = !words.empty(),
                 hasNumbers = !numbers.empty();

      if(hasWords) dump(words);

      // The pipe separator should only be shown if both
      // types of data are present in the input.
      if(hasWords && hasNumbers) std::cout << '|';

      if(hasNumbers) dump(numbers);

      std::cout << '\n';

      words.clear();
      numbers.clear();
    }
  }
}
