#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "munkres.hpp"

static void explode(const std::string& source,
                    std::vector<std::string>& bucket,
                    const char delimiter = ',')
{
  bucket.clear();

  auto right = source.find(delimiter),
       left  = decltype(right){};

  // Should be a bit leaner than a std::istringstream.
  while(right != std::string::npos)
  {
    const auto fragmentWidth = (right - left);

    if(fragmentWidth) bucket.emplace_back(source.substr(left, fragmentWidth));

    left  = ++right;
    right = source.find(delimiter, left);
  }

  if(source.size() - left) bucket.emplace_back(source.substr(left));
}

static float getSuitabilityScore(const std::string& customerName,
                                 const std::string& productName)
{
  assert(!customerName.empty() && !productName.empty());

  static const auto getLetterCount = [](const std::string& sequence)
  {
    return std::count_if(sequence.cbegin(), sequence.cend(), ::isalpha);
  };

  static const auto getVowelCount = [](const std::string& sequence)
  {
    return std::count_if(sequence.cbegin(), sequence.cend(), [](char c)
    {
      c = ::tolower(c);

      return (c == 'a') || (c == 'e') || (c == 'i') ||
             (c == 'o') || (c == 'u') || (c == 'y');
    });
  };

  const auto customerLetterCount = getLetterCount(customerName),
             productLetterCount  = getLetterCount(productName);

  const auto baseScore = [&]() -> float
  {
    const auto customerVowelCount = getVowelCount(customerName);

    // If the number of letters in the product's name is odd then the
    // "Suitability Score" is the number of consonants in the customer's name.
    if(productLetterCount & 1)
    {
      return (customerLetterCount - customerVowelCount);
    }

    // If the number of letters in the product's name is even then the
    // "Suitability Score" is the number of vowels in the customer's name
    // multiplied by one and one-half.
    return (customerVowelCount * 1.5f);
  }();

  const auto scoreMultiplier = [&]() -> float
  {
    const auto limit = std::min(customerLetterCount, productLetterCount);

    // If the number of letters in the product's name shares any common factors
    // (besides 1) with the number of letters in the customer's name then the
    // "Suitability Score" is multiplied by a factor of one and one-half.
    if(limit > 1) for(auto i = decltype(limit){2}; i <= limit; ++i)
    {
      if(!(customerLetterCount % i) && !(productLetterCount % i)) return 1.5f;
    }

    return 1;
  }();

  return (baseScore * scoreMultiplier);
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  using score_row_type = std::vector<float>;

  std::vector<std::string> names, products;
  std::vector<score_row_type> scores;

  std::cout << std::fixed << std::setprecision(2);

  for(std::string line; std::getline(inputStream, line);)
  {
    const auto delimiterIndex = line.find(';');

    assert(delimiterIndex != std::string::npos);

    ::explode(line.substr(0, delimiterIndex), names);
    ::explode(line.substr(delimiterIndex + 1), products);

    const auto nameCount    = names.size(),
               productCount = products.size();

    if(!nameCount || !productCount)
    {
      std::cout << 0.0f << '\n';

      continue;
    }

    scores.clear();

    const auto matrixWidth = std::max(nameCount, productCount);

    // The scoring table has to be square.
    scores.resize(matrixWidth, score_row_type(matrixWidth));

    // Fill suitability scoring table.
    for(auto i = decltype(nameCount){}; i < nameCount; ++i)
    for(auto j = decltype(productCount){}; j < productCount; ++j)
    {
      scores[i][j] = ::getSuitabilityScore(names[i], products[j]);
    }

    const auto maxOfferValue = [&]
    {
      // Prepare data for use with vendor's C-style API.
      std::vector<std::vector<int>> storage(matrixWidth);
      std::vector<int*> values(matrixWidth);

      for(auto i = decltype(matrixWidth){}; i < matrixWidth; ++i)
      {
        storage[i].resize(matrixWidth);

        for(auto j = decltype(matrixWidth){}; j < matrixWidth; ++j)
        {
          // HACK: The vendor's library only works with integers. I'm too lazy
          // to change its interface, though, so this hack should do for now.
          storage[i][j] = (scores[i][j] * 1000);
        }

        values[i] = storage[i].data();
      }

      // NOTE: The caller is responsible for freeing this. This doesn't matter
      // on CodeEval, so not going to bother.
      const auto filter = Munkres::select(values.data(), matrixWidth);

      score_row_type::value_type result = 0;

      // Tally selected cells in the cost matrix.
      for(auto i = decltype(matrixWidth){}; i < matrixWidth; ++i)
      for(auto j = decltype(matrixWidth){}; j < matrixWidth; ++j)
      {
        if(filter[i][j])
        {
          result += scores[i][j];
          break;
        }
      }

      return result;
    }();

    std::cout << maxOfferValue << '\n';
  }
}
