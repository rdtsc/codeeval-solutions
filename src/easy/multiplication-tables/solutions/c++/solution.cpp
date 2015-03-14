#include <iomanip>
#include <iostream>

int main()
{
  for(auto x = 1; x <= 12; ++x)
  {
    std::cout << x;

    for(auto y = 2; y <= 12; ++y) std::cout << std::setw(4) << x * y;

    std::cout << '\n';
  }
}
