#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(std::string line; std::getline(inputStream, line); std::cout << '\n')
  {
    // Reverse the input line: "abc def ghi" -> "ihg fed cba"
    std::reverse(line.begin(), line.end());

    for(std::istringstream tokenizer(line); tokenizer >> line;)
    {
      using output_iterator = std::ostreambuf_iterator<char>;

      // Reconstruct each word: "ihg" -> "ghi"
      std::copy(line.rbegin(), line.rend(), output_iterator(std::cout));

      if(!tokenizer.eof()) std::cout << ' ';
    }
  }
}
