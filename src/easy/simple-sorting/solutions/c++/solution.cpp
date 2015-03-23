#include <algorithm>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::cout << std::fixed << std::setprecision(3);

  std::vector<float> items;

  for(decltype(items)::value_type n = 0; inputStream >> n;)
  {
    items.push_back(n);

    switch(inputStream.peek()) case '\n': case EOF:
    {
      std::sort(items.begin(), items.end());

      for(const auto& n : items) std::cout << n << ' ';

      std::cout << '\n';
      items.clear();
    }
  }
}
