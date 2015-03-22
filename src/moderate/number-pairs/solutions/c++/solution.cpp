#include <cassert>
#include <fstream>
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

  std::vector<unsigned> terms;

  for(decltype(terms)::value_type n = 0; inputStream >> n; inputStream.ignore())
  {
    terms.push_back(n);

    switch(inputStream.peek()) case '\n': case EOF:
    {
      const auto targetSum = terms.back();

      assert(targetSum);
      terms.pop_back();

      const auto termCount = terms.size();

      // We should bail early if the last two terms in the sorted sequence are
      // less than the target sum since there's no meaningful work to do.
      if(termCount >= 2 &&
        (terms[termCount - 1] + terms[termCount - 2]) < targetSum)
      {
        std::cout << "NULL\n";
      }

      else
      {
        const auto foundPairs = [&]
        {
          bool foundPairs = false;

          for(auto i = decltype(termCount){}; i < termCount; ++i)
          for(auto j = i + 1; j < termCount; ++j)
          {
            const auto pairSum = terms[i] + terms[j];

            // Since the sequence is sorted, we can bail early if the current
            // pair's sum exceeds the target sum without checking the rest of
            // the sequence.
            if(pairSum > targetSum) break;

            else if(pairSum == targetSum)
            {
              if(foundPairs) std::cout << ';';
              else if(!foundPairs) foundPairs = true;

              std::cout << terms[i] << ',' << terms[j];
              break;
            }
          }

          return foundPairs;
        }();

        if(!foundPairs) std::cout << "NULL";

        std::cout << '\n';
      }

      terms.clear();
    }
  }
}
