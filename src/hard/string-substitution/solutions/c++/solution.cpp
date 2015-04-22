#include <cassert>
#include <fstream>
#include <iostream>
#include <locale>
#include <string>
#include <utility>
#include <vector>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Add commas and semicolons to the list of stream delimiters.
  inputStream.imbue(std::locale(inputStream.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    categories[','] = categories[';'] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  std::vector<std::string> data;

  for(std::string subject; inputStream >> subject;)
  {
    data.push_back(subject);

    switch(inputStream.peek()) case '\n': case EOF:
    {
      // Subject + N replacement pairs result in an odd number of elements.
      assert(data.size() & 1);

      subject = data.front();

      auto haystack = data.begin();

      // Run through the replacement pairs from left to right.
      for(auto needle = data.cbegin() + 1, dataEnd = data.cend();
          needle != dataEnd;
          needle += 2)
      {
        // Replace all occurrences of the needle in the subject and
        // the bookkeeping haystack.
        for(auto haystackPos = haystack->find(*needle);
            haystackPos != std::string::npos;
            haystackPos = haystack->find(*needle, haystackPos))
        {
          const auto replacement = needle + 1;
          const auto needleSize  = needle->size();

          subject.replace(haystackPos, needleSize, *replacement);
          haystack->replace(haystackPos, needleSize, replacement->size(), ' ');
        }
      }

      std::cout << subject << '\n';

      data.clear();
    }
  }
}
