#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  using value_type = unsigned;

  value_type n = 0;

  for(inputStream >> n; inputStream >> n;)
  {
    const value_type upperLimit = ::sqrt(n);

    const auto result = [&]
    {
      unsigned result = 0;

      for(auto i = decltype(upperLimit){}; i <= upperLimit; ++i)
      {
        const value_type value = (n - i * i),
                         test  = ::sqrt(value);

        if(test < i) return result;

        else if(test * test == value) ++result;
      }

      return result;
    }();

    std::cout << result << '\n';
  }
}
