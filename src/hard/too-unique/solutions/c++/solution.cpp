#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

template<typename T> struct Metrics {T w, h;};

static std::vector<std::string>
getRedactedMatrix(const std::vector<std::string>& matrix)
{
  assert(!matrix.empty());

  using Metrics = Metrics<std::size_t>;

  // Possible rectangular filter sizes.
  static const std::vector<Metrics> filters[] =
  {
    #define V(Area, ...) {{__VA_ARGS__}}

    V(26, {2, 13}),
    V(24, {2, 12}, {3, 8}, {4, 6}),
    V(22, {2, 11}),
    V(21, {3,  7}),
    V(20, {2, 10}, {4, 5}),
    V(18, {2,  9}, {3, 6}),
    V(16, {2,  8}),
    V(15, {3,  5}),
    V(14, {2,  7}),
    V(12, {2,  6}, {3, 4}),
    V(10, {2,  5}),
    V( 8, {2,  4}),
    V( 6, {2,  3})

    #undef V
  };

  const auto n = matrix.front().size(), // Matrix width.
             m = matrix.size();         // Matrix height.

  auto redactedMatrix = matrix;

  // Redact unique submatrices of the given size.
  const auto redact = [&](const std::size_t w, const std::size_t h)
  {
    if((w > n) || (h > m)) return false;

    const auto isUnique = [&](const std::size_t i, const std::size_t j)
    {
      std::uint_least32_t history = 0;

      for(auto y = i; y < (h + i); ++y)
      for(auto x = j; x < (w + j); ++x)
      {
        const std::size_t symbol = (matrix[y][x] - 'a');

        if(history & (1 << symbol)) return false;

        history |= (1 << symbol);
      }

      return true;
    };

    bool result = false;

    // This problem's matrices are small; brute-force should be fast enough.
    for(std::size_t i = 0; i <= (m - h); ++i)
    for(std::size_t j = 0; j <= (n - w); ++j)
    {
      if(isUnique(i, j))
      {
        result = true;

        for(auto y = i; y < (h + i); ++y)
        for(auto x = j; x < (w + j); ++x)
        {
          redactedMatrix[y][x] = '*';
        }
      }
    }

    return result;
  };

  // Try redacting the matrix starting from filters with the largest areas.
  for(const auto& filter : filters)
  {
    bool redacted = false;

    for(const auto& metrics : filter)
    {
      if(redact(metrics.w, metrics.h)) redacted = true;
      if(redact(metrics.h, metrics.w)) redacted = true;
    }

    if(redacted) break;
  }

  return redactedMatrix;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::vector<std::string> rows;

  using input_iterator = std::istream_iterator<std::string>;

  // Fetch base matrix.
  std::copy(input_iterator(inputStream), input_iterator(),
    std::back_inserter(rows));

  assert(rows.size() >= 6);

  assert(std::all_of(rows.cbegin(), rows.cend(), [&](const std::string& row)
  {
    const bool isWellFormed = (rows.front().size() == row.size());

    constexpr auto notFound = std::string::npos;

    const auto isLowerAlpha =
      (row.find_first_not_of("abcdefghijklmnopqrstuvwxyz") == notFound);

    return ((row.size() >= 12) && isWellFormed && isLowerAlpha);
  }));

  // Dump redacted matrix.
  for(const auto& row : getRedactedMatrix(rows)) std::cout << row << '\n';
}
