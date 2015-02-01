#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <sstream>
#include <vector>

static std::string decryptMessage(const std::string& encrypted)
{
  std::istringstream tokenizer(encrypted);

  // The problem statement guarantees that the message will consist of at least
  // two words and that all characters are padded with the same offset. Given
  // the above, we can simply look for a character with the smallest value in
  // the encrypted message and be guaranteed that it's a space (0x20) since
  // that's the lowest printable character in the ASCII table. We can therefore
  // safely ignore the hint information provided at the front of the input line
  // by jumping straight to the encrypted payload.
  {
    static constexpr auto ignoreLimit =
      std::numeric_limits<std::streamsize>::max();

    tokenizer.ignore(ignoreLimit, '|');
    tokenizer.ignore(ignoreLimit, '|');
  }

  const auto values = [&]
  {
    using value_type = unsigned;
    using input_iterator = std::istream_iterator<value_type>;

    // Read the encrypted data.
    std::vector<value_type> values{input_iterator(tokenizer), input_iterator()};

    const auto valuesBegin = values.begin(), valuesEnd = values.end();

    // Find the lowest-valued character which is equal to: 0x20 + offset
    const auto encryptionOffset = [&]
    {
      const auto result = std::min_element(valuesBegin, valuesEnd);

      if(result == valuesEnd) return 0U;

      // Reconstruct the encryption offset.
      return (*result - ' ');
    }();

    if(!encryptionOffset) return values;

    // Decrypt the characters.
    std::transform(valuesBegin, valuesEnd, valuesBegin, [&](const value_type c)
    {
      return (c - encryptionOffset);
    });

    return values;
  }();

  return std::string(values.cbegin(), values.cend());
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
    std::cout << decryptMessage(line) << '\n';
  }
}
