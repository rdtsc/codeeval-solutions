#include <cassert>
#include <deque>
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

  std::deque<unsigned> sequence;

  auto unserializeSequence = [&](const std::string& source)
  {
    static std::istringstream tokenizer;

    sequence.clear();
    tokenizer.clear();
    tokenizer.str(source);

    for(decltype(sequence)::value_type term = 0; tokenizer >> term;)
    {
      sequence.push_back(term);
    }
  };

  for(std::string line; std::getline(inputStream, line);)
  {
    unserializeSequence(line);

    auto term = sequence.front(), termPrev = term;
    auto termCount = 1U;

    sequence.pop_front();

    for(const auto value : sequence)
    {
      term = value;

      // Series being continued.
      if(term == termPrev) ++termCount;

      // Series interrupted.
      else
      {
        std::cout << termCount << ' ' << termPrev << ' ';

        termPrev  = term;
        termCount = 1;
      }
    }

    // Account for the last set.
    std::cout << termCount << ' ' << term << '\n';
  }
}
