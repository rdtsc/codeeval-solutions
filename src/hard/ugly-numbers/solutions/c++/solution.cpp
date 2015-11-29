#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

static void inflate(std::string& target,
                    const char padding = ' ',
                    const std::size_t count = 1)
{
  assert(!target.empty());

  auto length = target.size();

  for(auto i = decltype(length){1}; i < (length++); i += 2)
  {
    target.insert(i, count, padding);
  }
}

template<typename T, std::size_t CellCount = 16>
static T evaluate(const std::string& expression)
{
  std::array<T, CellCount> values = {0};
  std::array<char, CellCount> tokens = {0};

  std::size_t valueCount = 0,
              tokenCount = 0;

  typename decltype(values)::value_type value = 0;

  for(const auto c : expression)
  {
    if(::isdigit(c)) value = (value * 10) + (c - '0');

    else if(c == '+' || c == '-')
    {
      assert(valueCount < values.size());
      assert(tokenCount < tokens.size());

      values[valueCount++] = value;
      tokens[tokenCount++] = c;
      value = 0;
    }

    // Treat spaces as a concatenation operator.
    else if(c != ' ') assert(false);
  }

  assert(valueCount < values.size());

  values[valueCount++] = value;
  value = values.front();

  assert((tokenCount + 1) == valueCount);

  for(std::size_t i = 1, j = 0; i < valueCount; ++i) switch(tokens[j++])
  {
    case '+': value += values[i]; break;
    case '-': value -= values[i]; break;
  }

  return value;
}

template<typename T> static constexpr bool isUgly(const T n)
{
  return (!(n % 2) || !(n % 3) || !(n % 5) || !(n % 7));
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  static constexpr char operators[] = {'+', ' ', '-'};

  for(std::string line; std::getline(inputStream, line);)
  {
    // As per the problem statement.
    assert(!line.empty() && (line.size() < 14));
    assert(std::all_of(line.cbegin(), line.cend(), ::isdigit));

    // There are floor(log10(n))) slots to insert operators.
    const std::size_t operatorCount = (line.size() - 1);

    // Add spaces between digits to make room for operators; "123" -> "1 2 3".
    ::inflate(line);

    // There are 3^floor(log10(n)) possible expressions.
    const std::size_t expressionCount = std::pow(3, operatorCount);

    std::array<std::size_t, 16> select = {0};

    std::size_t uglyExpressionCount = 0;

    for(auto i = decltype(expressionCount){}; i < expressionCount; ++i)
    {
      // Inject the selected operators.
      for(std::size_t j = 0, k = 1; j < operatorCount; ++j, k += 2)
      {
        line[k] = operators[select[j]];
      }

      // Select the next combination of operators.
      for(int j = (operatorCount - 1); j >= 0; --j)
      {
        if(++select[j] != sizeof operators) break;

        select[j] = 0;
      }

      uglyExpressionCount += ::isUgly(::evaluate<std::int_least64_t>(line));
    }

    std::cout << uglyExpressionCount << '\n';
  }
}
