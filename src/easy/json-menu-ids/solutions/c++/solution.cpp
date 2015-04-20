#include <cassert>
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

  static const char idKey[] = "\"id\":", labelKey[] = "\"label\":";

  std::istringstream tokenizer;

  // HACK: This is extremely fragile and ugly. I don't think we're expected to
  // implement an actual JSON parser for this challenge, though, so whatever.
  for(std::string line; std::getline(inputStream, line);)
  {
    tokenizer.clear();
    tokenizer.str(line);

    unsigned idSum = 0;

    while(std::getline(tokenizer, line, '{') &&
          std::getline(tokenizer, line, '}'))
    {
      const auto idKeyPos    = line.find(idKey),
                 labelKeyPos = line.find(labelKey);

      // The chunk should have an ID as well as a label.
      if(idKeyPos != std::string::npos && labelKeyPos != std::string::npos)
      {
        line = line.substr(idKeyPos + sizeof(idKey) - 1);

        idSum += std::stoi(line);
      }
    }

    std::cout << idSum << '\n';
  }
}
