#include <cmath>
#include <iostream>
#include <type_traits>

// Brute-force search. Fast enough for this problem.
template<typename T> static inline bool isPrime(const T n)
{
  if(n <= 1)        return false; // 0 and 1 -> n/a.
  else if(n < 4)    return true;  // 2 and 3 -> prime.
  else if(!(n & 1)) return false; // Even numbers -> composite.
  else if(n < 9)    return true;  // 5 and 7 -> prime.
  else if(!(n % 3)) return false; // Multiples of 3 -> composite.

  const T limit = std::sqrt(n);

  // Primes >3 are of the form (6n +/- 1).
  for(T i = 5; i <= limit; i += 6)
  {
    if(!(n % i) || !(n % (i + 2))) return false;
  }

  return true;
}

template<typename T> static inline bool isPalindrome(const T n)
{
  static_assert(std::is_unsigned<T>::value, "Expecting an unsigned type.");

  T reversed = 0;

  for(T i = n; i; i /= 10)
  {
    reversed = (reversed * 10) + (i % 10);
  }

  return reversed == n;
}

int main()
{
  // Smallest prime under 1k.
  unsigned n = 997;

  // Find the largest prime palindrome under 1k.
  while(n -= 6)
  {
    if(::isPalindrome(n) && ::isPrime(n)) break;

    if(::isPalindrome(n - 2) && ::isPrime(n - 2))
    {
      n -= 2;
      break;
    }
  }

  std::cout << n << '\n';
}
