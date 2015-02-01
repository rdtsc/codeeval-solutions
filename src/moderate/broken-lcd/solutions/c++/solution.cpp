#include <bitset>
#include <cassert>
#include <cctype>
#include <climits>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

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
    const auto delimiterPos = line.rfind(';');

    assert(delimiterPos != std::string::npos);

    using byte = unsigned char;

    // Fetch the display damage masks.
    const auto lcd = [&]
    {
      std::vector<byte> result;

      std::istringstream tokenizer(line.substr(0, delimiterPos));

      for(std::string cell; tokenizer >> cell;)
      {
        result.push_back((std::bitset<CHAR_BIT>(cell)).to_ulong());
      }

      return result;
    }();

    // Convert the digits of the number to be shown into segment toggle masks.
    const auto digits = [&]
    {
      std::vector<byte> result;

      static const byte digitMasks[] =
      {
        /*
          7-segment digit mappings with the 8th segment (dot) turned off.

          0: 11111100   1: 01100000   2: 11011010   3: 11110010   4: 01100110
          5: 10110110   6: 10111110   7: 11100000   8: 11111110   9: 11110110
        */

        0xFC, 0x60, 0xDA, 0xF2, 0x66,
        0xB6, 0xBE, 0xE0, 0xFE, 0xF6
      };

      for(const auto c : line.substr(delimiterPos + 1))
      {
        if(::isdigit(c)) result.push_back(digitMasks[c - '0']);

        // The decimal belongs to the previous number's 8th segment.
        else if(c == '.') result.back() += 1;
      }

      return result;
    }();

    const auto result = [&]
    {
      const auto lcdWidth = lcd.size(), digitCount = digits.size();

      // Checks whether it's possible to display the number from the
      // current position on the LCD, moving LTR.
      auto isDisplayable = [&](const std::size_t lcdOffset)
      {
        for(std::size_t i = 0; i < digitCount; ++i)
        {
          if((lcd[lcdOffset + i] & digits[i]) != digits[i]) return false;
        }

        return true;
      };

      // Attempt to place the digits on the display from left to right.
      for(std::size_t i = 0; i < lcdWidth - digitCount + 1; ++i)
      {
        if(isDisplayable(i)) return true;
      }

      return false;
    }();

    std::cout << result << '\n';
  }
}
