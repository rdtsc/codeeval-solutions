#include <cmath>
#include <iostream>

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

int main()
{
  // 2 and 3 are included.
  unsigned primeSum = 5;

  // Sum the first 1k primes.
  for(unsigned i = 5, primeCount = 2; primeCount < 1000; i += 6)
  {
    if(::isPrime(i))
    {
      ++primeCount;
      primeSum += i;
    }

    if(::isPrime(i + 2))
    {
      ++primeCount;
      primeSum += i + 2;
    }
  }

  std::cout << primeSum << '\n';
}
