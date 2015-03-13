#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  auto eval = [](const std::string& digits, const std::string& pattern)
  {
    bool shouldAdd = false;
    std::int_fast32_t numbers[2] = {};
    std::size_t i = 0;

    for(const char c : pattern)
    {
      // Lowercase letters map to digits.
      if(c >= 'a' && c <= 'z')
      {
        numbers[i] = (numbers[i] * 10) + (digits[c - 'a'] - '0');
      }

      else
      {
        ++i;

        assert(i < 2);

        if(c == '+') shouldAdd = true;
      }
    }

    // Expecting at most one operator in the pattern.
    assert(i == 1);

    return (shouldAdd) ? (numbers[0] + numbers[1]):
                         (numbers[0] - numbers[1]);
  };

  for(std::string digits, pattern; inputStream >> digits >> pattern;)
  {
    std::cout << eval(digits, pattern) << '\n';
  }
}
