#include <array>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

template<std::size_t TapeLength> class Interpreter
{
  public: Interpreter() : data()
  {
    static_assert(TapeLength >= 64, "Tape length should be longer.");

    static_assert(!(TapeLength & (TapeLength - 1)),
                  "Tape length should be a power of 2.");
  }

  public: void run(std::ostream& outputStream = std::cout)
  {
    assert(!this->instructions.empty());

    const auto instructionsBegin = this->instructions.cbegin(),
               instructionsEnd   = this->instructions.cend();

    auto cell = std::next(this->data.begin(), (TapeLength / 2));

    // As per: https://en.wikipedia.org/wiki/Brainfuck#Commands
    for(auto i = instructionsBegin; i != instructionsEnd; ++i) switch(*i)
    {
      // Movement; unsafe.
      case '>': ++cell; break;
      case '<': --cell; break;

      // Mutation.
      case '+': ++*cell; break;
      case '-': --*cell; break;

      // Output.
      case '.': outputStream << *cell; break;

      // Flow control.
      case '[': if(!*cell) for(unsigned parity = 1; parity;)
      {
        switch(*++i)
        {
          case '[': ++parity; break;
          case ']': --parity; break;
        }
      }
      break;

      case ']': for(unsigned parity = 0;;)
      {
        switch(*i--)
        {
          case '[': ++parity; break;
          case ']': --parity; break;
        }

        if(!parity) break;
      }
      break;

      default: assert(false);
    }

    this->reset();
  }

  public: void reset()
  {
    this->instructions.clear();
    this->data.fill('\0');
  }

  private: friend std::istream& operator>>(std::istream& inputStream,
                                           Interpreter& interpreter)
  {
    return std::getline(inputStream, interpreter.instructions);
  }

  private: friend std::ostream& operator<<(std::ostream& outputStream,
                                           Interpreter& interpreter)
  {
    interpreter.run(outputStream);

    return outputStream;
  }

  private: std::string instructions;
  private: std::array<char, TapeLength> data;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(Interpreter<2048> interpreter; inputStream >> interpreter;)
  {
    std::cout << interpreter << '\n';
  }
}
