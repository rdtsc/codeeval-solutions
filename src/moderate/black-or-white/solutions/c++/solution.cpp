#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

struct ParkMatrix
{
  std::pair<std::size_t, unsigned> getSmallestUnitMetrics() const
  {
    assert(!this->rows.empty());

    std::pair<std::size_t, unsigned> result;

    const auto width = this->rows.front().size();

    // Expand the (square) search submatrix until we find one where the sums
    // of all such submatrices are equal.
    for(auto scope = decltype(width){1}; scope <= width; ++scope) if([&]
    {
      bool isFirstSubmatrix = true;

      for(decltype(scope) yMajor = 0; yMajor <= (width - scope); ++yMajor)
      for(decltype(scope) xMajor = 0; xMajor <= (width - scope); ++xMajor)
      {
        unsigned cellSum = 0;

        for(auto y = yMajor; y < (yMajor + scope); ++y)
        for(auto x = xMajor; x < (xMajor + scope); ++x)
        {
          cellSum += rows[y][x];

          if(!isFirstSubmatrix && (cellSum > result.second)) return false;
        }

        if(isFirstSubmatrix)
        {
          result.second = cellSum;
          isFirstSubmatrix = false;
        }

        else if(result.second != cellSum) return false;
      }

      result.first = scope;

      return true;
    }()) break;

    return result;
  }

  friend std::istream& operator>>(std::istream& inputStream, ParkMatrix& matrix)
  {
    auto& rows = matrix.rows;

    rows.clear();

    std::string line;

    if(!std::getline(inputStream, line)) return inputStream;

    std::string row;

    for(const auto c : line)
    {
      if(c == '0' || c == '1') row += (c - '0');

      else if(c == '|')
      {
        rows.push_back(row);
        row.clear();
      }
    }

    rows.push_back(row);

    // As per the problem statement.
    assert(rows.size() >= 2 && rows.size() <= 25);

    // Verify squareness.
    assert(std::all_of(rows.begin(), rows.end(), [&](const std::string& row)
    {
      return (!row.empty() && (row.size() == rows.front().size()));
    }));

    return inputStream;
  }

  std::vector<std::string> rows;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(ParkMatrix matrix; inputStream >> matrix;)
  {
    const auto result = matrix.getSmallestUnitMetrics();
    const auto width  = result.first;
    const auto count  = result.second;

    std::cout << width << 'x' << width << ", " << count << '\n';
  }
}
