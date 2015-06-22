#include <cassert>
#include <fstream>
#include <iostream>
#include <locale>
#include <string>

class CodeEvalFacet : public std::numpunct<char>
{
  protected: virtual std::string do_truename() const override
  {
    return "YES";
  }

  protected: virtual std::string do_falsename() const override
  {
    return "NO";
  }
};

static bool isBalanced(const std::string& message)
{
  assert(!message.empty());

  struct {int open, closed;} count = {0, 0};

  const auto messageBegin = message.cbegin(),
             messageEnd   = message.cend();

  for(auto i = messageBegin; i != messageEnd; ++i)
  {
    const char c = *i;

    // Are we part of a smiley?
    const auto noLeadingColon = ((i != messageBegin ? *(i - 1) : c) != ':');

    if(c == ')')
    {
      if(noLeadingColon && (--count.open < 0)) return false;

      if(--count.closed < 0) count.closed = 0;
    }

    else if(c == '(')
    {
      ++count.open;

      if(noLeadingColon) ++count.closed;
    }
  }

  return (!count.closed && count.open >= 0);
}

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

    // Ref-counted upstream.
    return new CodeEvalFacet();
  }()));

  for(std::string line; std::getline(inputStream, line);)
  {
    std::cout << ::isBalanced(line) << '\n';
  }
}
