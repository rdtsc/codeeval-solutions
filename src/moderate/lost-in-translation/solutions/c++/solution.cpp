#include <algorithm>
#include <cassert>
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

  using input_iterator  = std::istreambuf_iterator<char>;
  using output_iterator = std::ostreambuf_iterator<char>;

  std::transform(input_iterator(inputStream.rdbuf()),
                 input_iterator(),
                 output_iterator(std::cout),
                 [](const char c)
  {
    // The example in the problem statement explicitly covers all characters
    // but one. The following was quickly derived by hand and maps scrambled
    // alphabet positions to their corresponding plain-text characters.
    static const char* const mapping = "yhesocvxduiglbkrztnwjpfmaq";

    return (c >= 'a' && c <= 'z') ? (mapping[c - 'a']) : (c);
  });
}
