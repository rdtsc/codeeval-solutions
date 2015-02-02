#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

template<typename T> class Stack
{
  public: using value_type = T;

  public: void push(const T& value)
  {
    this->data.push_back(value);
  }

  public: T top() const
  {
    if(this->isEmpty()) throw std::out_of_range("Stack is empty.");

    return this->data.back();
  }

  public: void pop()
  {
    if(!this->isEmpty()) this->data.pop_back();
  }

  public: bool isEmpty() const
  {
    return this->data.empty();
  }

  private: std::vector<T> data;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  Stack<std::int_fast32_t> integers;

  for(decltype(integers)::value_type value = 0; inputStream >> value;)
  {
    integers.push(value);

    const auto peek = inputStream.peek();

    // No more values on the current line.
    if(peek == '\n' || peek == EOF)
    {
      for(;!integers.isEmpty(); integers.pop(), integers.pop())
      {
        std::cout << integers.top() << ' ';
      }

      std::cout << '\n';
    }
  }
}
