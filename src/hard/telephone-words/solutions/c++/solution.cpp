#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
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

  static const std::string keypad[] =
  {
    /* 0 */ "0",
    /* 1 */ "1",
    /* 2 */ "abc",
    /* 3 */ "def",
    /* 4 */ "ghi",
    /* 5 */ "jkl",
    /* 6 */ "mno",
    /* 7 */ "pqrs",
    /* 8 */ "tuv",
    /* 9 */ "wxyz"
  };

  constexpr std::size_t telephoneLength = 7;

  for(std::string digits, mapping(telephoneLength, '\0');
      std::getline(inputStream, digits); std::cout << '\n')
  {
    assert(digits.size() >= telephoneLength);
    assert(std::all_of(digits.cbegin(), digits.cend(), ::isdigit));

    // Phone numbers aren't too long and have a fixed length, so this hack
    // should be alright.

    #define NEXT(Index) for(const auto c##Index : keypad[digits[(Index)] - '0'])
    #define SAVE(Index) mapping[(Index)] = c##Index

    bool isFirstResult = true;

    NEXT(0) NEXT(1) NEXT(2) NEXT(3) NEXT(4) NEXT(5) NEXT(6)
    {
      if(!isFirstResult) std::cout << ',';
      else isFirstResult = false;

      SAVE(0); SAVE(1); SAVE(2); SAVE(3); SAVE(4); SAVE(5); SAVE(6);

      std::cout << mapping;
    }

    #undef SAVE
    #undef NEXT
  }
}
