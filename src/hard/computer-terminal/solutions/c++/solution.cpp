#include <array>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>

// TODO: Ownership is all over the place. This needs revision.
template<std::size_t Rows, std::size_t Columns> class FixedTerminal
{
  public: FixedTerminal() {}

  public: explicit FixedTerminal(std::istream& commandStream)
  {
    while(commandStream >> *this);
  }

  private: enum class CursorMode {Overwrite, Insert};

  private: struct Cursor
  {
    Cursor() : x(), y(), mode(CursorMode::Overwrite) {}

    void setPos(const std::size_t x, const std::size_t y)
    {
      if(x < Rows)    this->x = x;
      if(y < Columns) this->y = y;
    }

    void reset() {this->setPos(0, 0);}
    void home()  {this->x = 0;}

    void left()  {if(x) --this->x;}
    void up()    {if(y) --this->y;}

    void right() {if(x < Columns - 1) ++this->x;}
    void down()  {if(y < Rows - 1)    ++this->y;}

    std::size_t x, y;
    CursorMode mode;
  } cursor;

  private: struct Row
  {
    Row() : buffer(Columns, ' ') {}

    void clear() {this->clearRight(0);}

    void clearRight(const std::size_t offset)
    {
      std::fill(std::begin(this->buffer) + offset, std::end(this->buffer), ' ');
    }

    friend std::ostream& operator<<(std::ostream& outputStream, const Row& row)
    {
      return outputStream << row.buffer;
    }

    std::string buffer;
  };

  private: std::array<Row, Rows> buffer;

  public: void unserialize(const std::string& commands)
  {
    for(std::string::size_type i = 0, iEnd = commands.size(); i < iEnd; ++i)
    {
      const char c = commands[i], peek = (i + 1 < iEnd) ? commands[i + 1] : EOF;

      if(c == '^' && ++i) switch(peek)
      {
        // Destruction.
        case 'c': this->clearScreen();      break;
        case 'e': this->trimRightColumns(); break;

        // Cursor positioning.
        case 'h': this->cursor.reset(); break;
        case 'b': this->cursor.home();  break;
        case 'r': this->cursor.right(); break;
        case 'd': this->cursor.down();  break;
        case 'l': this->cursor.left();  break;
        case 'u': this->cursor.up();    break;

        // Cursor modes.
        case 'i': this->cursor.mode = CursorMode::Insert;    break;
        case 'o': this->cursor.mode = CursorMode::Overwrite; break;

        // Escaped characters.
        case '^': this->write('^'); break;

        // Explicit cursor positioning.
        default: if(::isdigit(peek) && ::isdigit(commands[i + 1]))
        {
          this->cursor.setPos(commands[++i] - '0', peek - '0');
        }
      }

      else this->write(c);
    }
  }

  private: void write(const char c)
  {
    assert(::isprint(c));

    auto& rowBuffer = this->buffer[this->cursor.y].buffer;

    switch(this->cursor.mode)
    {
      case CursorMode::Overwrite:
      {
        rowBuffer[this->cursor.x] = c;
      }
      break;

      case CursorMode::Insert:
      {
        rowBuffer.insert(rowBuffer.begin() + this->cursor.x, c);
        rowBuffer.pop_back();
      }
      break;
    }

    this->cursor.right();
  }

  private: void clearScreen()
  {
    for(auto& row : this->buffer) row.clear();
  }

  private: void trimRightColumns()
  {
    this->buffer[this->cursor.y].clearRight(this->cursor.x);
  }

  private: friend std::istream& operator>>(std::istream& inputStream,
                                           FixedTerminal& terminal)
  {
    std::string commands;

    std::getline(inputStream, commands);

    terminal.unserialize(commands);

    return inputStream;
  }

  private: friend std::ostream& operator<<(std::ostream& outputStream,
                                           const FixedTerminal& terminal)
  {
    for(const auto& row : terminal.buffer)
    {
      outputStream << row << '\n';
    }

    return outputStream;
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

  std::cout << FixedTerminal<10, 10>(inputStream);
}
