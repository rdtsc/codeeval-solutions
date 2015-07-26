#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

template<typename T> struct Matrix2d
{
  using value_type = T;
  using row_type = std::vector<T>;

  explicit Matrix2d(std::istream& inputStream)
  {
    inputStream >> (*this);
  }

  T getLargestSubmatrixSum() const
  {
    const auto width = this->data.size();

    assert(width);

    // Max row sum based on Kadane's algorithm.
    const auto getMaxRowSum = [](const row_type& row)
    {
      auto maxSum = row.front();
      auto localMax = maxSum;

      const auto rowEnd = row.cend();

      for(auto i = std::next(row.cbegin()); i != rowEnd; ++i)
      {
        localMax = std::max(*i, (localMax + *i));
        maxSum   = std::max(maxSum, localMax);
      }

      return maxSum;
    };

    row_type scratch(width);

    T result = 0;

    // NOTE: Should probably rotate the matrix for better data locality.
    for(auto i = decltype(width){0}; i < width; ++i)
    {
      std::fill(scratch.begin(), scratch.end(), 0);

      for(auto x = i; x < width; ++x)
      {
        for(auto y = decltype(width){}; y < width; ++y)
        {
          scratch[y] += this->data[y][x];
        }

        result = std::max(result, getMaxRowSum(scratch));
      }
    }

    return result;
  }

  friend std::istream& operator>>(std::istream& inputStream, Matrix2d& matrix)
  {
    row_type row;

    // Fetch first row.
    for(T n = 0; inputStream >> n;)
    {
      row.push_back(n);

      const char peek = inputStream.peek();

      if(peek == '\n' || peek == EOF) break;
    }

    assert(!row.empty());

    const auto width = row.size();

    matrix.data.reserve(width);
    matrix.data.emplace_back(row);

    // Fetch the remaining rows.
    for(auto i = decltype(width){}; i < (width - 1); ++i)
    {
      row.clear();

      for(auto j = decltype(i){}; j < width; ++j)
      {
        T n = 0;

        if(!(inputStream >> n)) assert(false);

        row.push_back(n);
      }

      matrix.data.emplace_back(row);
    }

    return inputStream;
  }

  std::vector<row_type> data;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::cout << Matrix2d<int>(inputStream).getLargestSubmatrixSum() << '\n';
}
