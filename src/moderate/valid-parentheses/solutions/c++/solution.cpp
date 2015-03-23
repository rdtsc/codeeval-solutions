#include <cassert>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>

static inline bool isBalancedParentheses(const std::string& arrangement)
{
  assert(!arrangement.empty());

  // Reusing storage yields a much lower memory footprint on CodeEval.
  static std::stack<char> parentheses;

  while(!parentheses.empty()) parentheses.pop();

  for(const char c : arrangement) switch(c)
  {
    case '(': case '{': case '[':
    {
      parentheses.push(c);
    }
    break;

    case ')': case '}': case ']':
    {
      // The '(' and ')' characters are directly adjacent to each other in the
      // ASCII table, while the "[]" and "{}" pairs have an extra character in
      // between.
      const auto offset = ((c == ')') ? 1 : 2);

      if(!parentheses.empty() && (parentheses.top() + offset == c))
      {
        parentheses.pop();
      }

      else return false;
    }
  }

  return parentheses.empty();
}

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
    std::cout << (::isBalancedParentheses(line) ? "True" : "False") << '\n';
  }
}
