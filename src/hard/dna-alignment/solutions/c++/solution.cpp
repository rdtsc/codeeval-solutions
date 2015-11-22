#include <algorithm>
#include <cassert>
#include <climits>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <locale>
#include <string>
#include <type_traits>
#include <vector>

template<typename T> struct ScoringSchema
{
  using value_type = T;

  #define FIELD(Name) T Name : CHAR_BIT

  FIELD(match);
  FIELD(mismatch);
  FIELD(gapOpen);
  FIELD(gapExtend);

  #undef FIELD
};

template<typename T> static T getAlignmentScore(const std::string& lhs,
                                                const std::string& rhs,
                                                const ScoringSchema<T>& schema)
{
  static_assert(std::is_signed<T>::value, "Underlying type should be signed.");

  const auto lhsSize = lhs.size(),
             rhsSize = rhs.size();

  using row_type = std::vector<T>;

  // Best alignment of lhs/rhs.
  std::vector<row_type> a((lhsSize + 1), row_type(rhsSize + 1));

  // Best alignment of gap/rhs.
  auto b = a;

  // Best alignment of lhs/gap.
  auto c = a;

  const auto getAffineGap = [&](const decltype(lhsSize) n)
  {
    return ((n - 1) * schema.gapExtend) + schema.gapOpen;
  };

  const auto getCellValue = [&](const decltype(lhsSize) n)
  {
    return (schema.gapOpen + schema.gapExtend) * n;
  };

  // Prepare first rows.
  for(auto i = decltype(rhsSize){1}; i <= rhsSize; ++i)
  {
    a[0][i] = getCellValue(i);
    b[0][i] = getCellValue(i);
    c[0][i] = getAffineGap(i);
  }

  // Prepare first columns.
  for(auto i = decltype(lhsSize){1}; i <= lhsSize; ++i)
  {
    a[i][0] = getCellValue(i);
    b[i][0] = getAffineGap(i);
    c[i][0] = getCellValue(i);
  }

  const auto getScore = [&](const decltype(lhsSize) i,
                            const decltype(rhsSize) j)
  {
    return (lhs[i - 1] == rhs[j - 1]) ? (schema.match) : (schema.mismatch);
  };

  for(auto i = decltype(lhsSize){1}; i <= lhsSize; ++i)
  for(auto j = decltype(rhsSize){1}; j <= rhsSize; ++j)
  {
    // Aligned.
    a[i][j] = getScore(i, j) + std::max({a[i - 1][j - 1],
                                         b[i - 1][j - 1],
                                         c[i - 1][j - 1]});

    // Gap.
    b[i][j] = std::max(a[i - 1][j] + schema.gapOpen,
                       b[i - 1][j] + schema.gapExtend);

    // Gap.
    c[i][j] = std::max(a[i][j - 1] + schema.gapOpen,
                       c[i][j - 1] + schema.gapExtend);
  }

  return a[lhsSize][rhsSize];
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Add vertical pipes to the list of stream delimiters.
  inputStream.imbue(std::locale(inputStream.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    categories['|'] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  static const ScoringSchema<short> schema =
  {
    +3, // Match.
    -3, // Mismatch.
    -8, // Gap opening.
    -1  // Gap extension.
  };

  for(std::string lhs, rhs; inputStream >> lhs >> rhs;)
  {
    // As per the problem statement.
    assert((lhs.size() <= 400) && (rhs.size() <= 400));

    std::cout << ::getAlignmentScore(lhs, rhs, schema) << '\n';
  }
}
