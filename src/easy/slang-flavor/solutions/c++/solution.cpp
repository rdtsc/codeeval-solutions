#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  static const char* const slang[] =
  {
    ", yeah!", ", this is crazy, I tell ya.", ", can U believe this?",
    ", eh?", ", aw yea.", ", yo.", "? No way!", ". Awesome!"
  };

  static constexpr auto slangTermCount = sizeof slang / sizeof *slang;

  std::size_t slangTermIndex = 0;
  bool shouldInjectSlang = false;

  using input_iterator = std::istreambuf_iterator<char>;

  std::for_each(input_iterator(inputStream.rdbuf()),
                input_iterator(),
                [&](const input_iterator::value_type c)
  {
    if(c == '.' || c == '!' || c == '?')
    {
      if(shouldInjectSlang)
      {
        std::cout << slang[slangTermIndex++ % slangTermCount];
      }

      else std::cout << c;

      // Every other occurance of the above should be replaced.
      shouldInjectSlang = !shouldInjectSlang;
    }

    else std::cout << c;
  });
}
