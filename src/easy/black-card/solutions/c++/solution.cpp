#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::istringstream tokenizer;
  std::list<std::string> names;

  for(std::string line; std::getline(inputStream, line);)
  {
    tokenizer.clear();
    tokenizer.str(line);
    names.clear();

    while(tokenizer >> line && line.front() != '|') names.emplace_back(line);

    assert((names.size() >= 3) && (names.size() <= 10));

    const auto target = [&]
    {
      unsigned result = 0;

      if(!(tokenizer >> result)) assert(false);

      assert((result >= 3) && (result <= 15));

      return --result;
    }();

    while(names.size() != 1)
    {
      names.erase(std::next(names.begin(), target % names.size()));
    }

    std::cout << names.front() << '\n';
  }
}
