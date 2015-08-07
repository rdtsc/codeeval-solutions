#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <vector>

template<typename T> struct MaxRow
{
  friend std::istream& operator>>(std::istream& inputStream, MaxRow& row)
  {
    // Get the next non-space character from the stream.
    const auto peek = [&]
    {
      while(inputStream.peek() == ' ') inputStream.ignore();

      return inputStream.peek();
    };

    auto& values = row.values;

    values.clear();

    // Fetch first row.
    for(T value = 0; inputStream >> value;)
    {
      values.push_back(value);

      if(peek() == '|') break;
    }

    if(!inputStream) return inputStream;

    assert(!values.empty() && (peek() == '|'));

    // Kill the trailing pipe after the first row.
    inputStream.ignore();

    std::size_t columnIndex = 0;

    // Retain the highest values in each column.
    for(T value = 0; inputStream >> value;)
    {
      if(value > values[columnIndex]) values[columnIndex] = value;

      switch(peek())
      {
        // End of table.
        case '\n': case EOF:
        {
          inputStream.clear();
          return inputStream;
        }
        break;

        // Next row.
        case '|':
        {
          columnIndex = 0;
          inputStream.ignore();
        }
        break;

        // Next column.
        default:
        {
          ++columnIndex;
          assert(columnIndex < values.size());
        }
        break;
      }
    }

    return inputStream;
  }

  friend std::ostream& operator<<(std::ostream& outputStream, const MaxRow& row)
  {
    for(const auto value : row.values)
    {
      outputStream << value << ' ';
    }

    return outputStream;
  }

  std::vector<T> values;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(MaxRow<int> maxRow; inputStream >> maxRow;)
  {
    std::cout << maxRow << '\n';
  }
}
