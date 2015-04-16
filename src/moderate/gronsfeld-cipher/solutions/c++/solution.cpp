#include <cassert>
#include <cstddef>
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

  static const std::string alphabet = " !\"#$%&'()*+,-./0123456789:<=>?@"
                                      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefg"
                                      "hijklmnopqrstuvwxyz";

  static const auto alphabetSize = alphabet.size();

  std::istringstream tokenizer;

  for(std::string line, key, message; std::getline(inputStream, line);)
  {
    tokenizer.clear();
    tokenizer.str(line);
    line.clear();

    std::getline(tokenizer, key, ';');
    std::getline(tokenizer, message);

    const auto keyBegin = key.cbegin(),
               keyEnd   = key.cend();

    auto keyChunk = keyBegin;

    for(const char c : message)
    {
      const std::size_t keyOffset = (*keyChunk - '0');

      auto alphaOffset = alphabet.find(c);

      assert(alphaOffset != std::string::npos);

      // Find the correct character in the alphabet.
      alphaOffset = (alphaOffset >= keyOffset) ?
                    (alphaOffset  - keyOffset) :
                    (alphabetSize - (keyOffset - alphaOffset));

      line.push_back(alphabet[alphaOffset]);

      // The key is circular.
      if(++keyChunk == keyEnd) keyChunk = keyBegin;
    }

    std::cout << line << '\n';
  }
}
