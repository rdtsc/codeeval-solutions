#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(std::string line; std::getline(inputStream, line);)
  {
    if(line.empty()) continue;

    // Fetch the number of elements in the sequence.
    const auto n = std::stoul(line);
    const auto delimiterPos = line.find(';');

    assert(delimiterPos != std::string::npos);

    // Sum the comma-separated terms in the array.
    const auto arraySum = [&]
    {
      auto result = decltype(n){};

      std::istringstream tokenizer(line.substr(delimiterPos + 1));

      for(decltype(result) term = 0; tokenizer >> term; result += term)
      {
        if(tokenizer.peek() == ',') tokenizer.ignore();
      }

      return result;
    }();

    // Expected sum for the arithmetic sequence: [0, N-2]
    const auto expectedArraySum = (n - 1) * (n - 2) / 2;

    // The difference between the manually tallied sum and the expected sum
    // exposes the duplicate term that's hidden away in the input array.
    std::cout << arraySum - expectedArraySum << '\n';
  }
}
