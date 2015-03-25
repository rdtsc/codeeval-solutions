#include <math.h>
#include <stdbool.h>
#include <stdio.h>

// Brute-force search. Fast enough for this problem.
static inline bool isPrime(const unsigned n)
{
  if(n <= 1)        return false; // 0 and 1 -> n/a.
  else if(n < 4)    return true;  // 2 and 3 -> prime.
  else if(!(n & 1)) return false; // Even numbers -> composite.
  else if(n < 9)    return true;  // 5 and 7 -> prime.
  else if(!(n % 3)) return false; // Multiples of 3 -> composite.

  const unsigned limit = sqrt(n);

  // Primes >3 are of the form (6n +/- 1).
  for(unsigned i = 5; i <= limit; i += 6)
  {
    if(!(n % i) || !(n % (i + 2))) return false;
  }

  return true;
}

int main(void)
{
  // 2 and 3 are included.
  unsigned primeSum = 5;

  // Sum the first 1k primes.
  for(unsigned i = 5, primeCount = 2; primeCount < 1000; i += 6)
  {
    if(isPrime(i))
    {
      ++primeCount;
      primeSum += i;
    }

    if(isPrime(i + 2))
    {
      ++primeCount;
      primeSum += i + 2;
    }
  }

  printf("%u\n", primeSum);
}
