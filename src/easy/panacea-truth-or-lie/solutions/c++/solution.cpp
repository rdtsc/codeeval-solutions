#include <cassert>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>

class CodeEvalFacet : public std::numpunct<char>
{
  protected: virtual std::string do_truename() const override
  {
    return "True";
  }

  protected: virtual std::string do_falsename() const override
  {
    return "False";
  }
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Set bools to be emitted as their textual equivalents in English.
  std::cout.imbue(std::locale(std::cout.getloc(), [&]
  {
    std::boolalpha(std::cout);

    return new CodeEvalFacet();
  }()));

  const auto getSum = [](std::string value, const unsigned base)
  {
    std::istringstream tokenizer(value);

    unsigned result = 0;

    while(tokenizer >> value)
    {
      result += std::stoul(value, nullptr, base);
    }

    // As per the problem statement.
    assert((result >= 60) && (result <= 1500));

    return result;
  };

  for(std::string lhs, rhs; std::getline(inputStream, lhs, '|') &&
                            std::getline(inputStream, rhs);)
  {
    std::cout << (getSum(lhs, 16) <= getSum(rhs, 2)) << '\n';
  }
}
