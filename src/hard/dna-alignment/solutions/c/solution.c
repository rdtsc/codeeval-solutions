#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
  short match;
  short mismatch;
  short gapOpen;
  short gapExtend;
} ScoringSchema;

static short getAlignmentScore(const char* const lhs,
                               const char* const rhs,
                               const ScoringSchema* const schema)
{
  assert(lhs && rhs && schema);

  const size_t lhsSize = strlen(lhs),
               rhsSize = strlen(rhs);

  #define MAX_WIDTH 401

  // Best alignment of lhs/rhs.
  static short a[MAX_WIDTH][MAX_WIDTH] = {{0}};

  // Best alignment of gap/rhs.
  static short b[MAX_WIDTH][MAX_WIDTH] = {{0}};

  // Best alignment of lhs/gap.
  static short c[MAX_WIDTH][MAX_WIDTH] = {{0}};

  #undef MAX_WIDTH

  #define GET_AFFINE_GAP(n) \
    ((((n) - 1) * schema->gapExtend) + schema->gapOpen)

  #define GET_CELL_VALUE(n) \
    ((schema->gapOpen + schema->gapExtend) * (n))

  #define GET_SCORE(i, j) \
    (lhs[(i) - 1] == rhs[(j) - 1] ? schema->match : schema->mismatch)

  // Prepare first rows.
  for(size_t i = 1; i <= rhsSize; ++i)
  {
    a[0][i] = GET_CELL_VALUE(i);
    b[0][i] = GET_CELL_VALUE(i);
    c[0][i] = GET_AFFINE_GAP(i);
  }

  // Prepare first columns.
  for(size_t i = 1; i <= lhsSize; ++i)
  {
    a[i][0] = GET_CELL_VALUE(i);
    b[i][0] = GET_AFFINE_GAP(i);
    c[i][0] = GET_CELL_VALUE(i);
  }

  for(size_t i = 1; i <= lhsSize; ++i)
  for(size_t j = 1; j <= rhsSize; ++j)
  {
    #define MAX2(a, b) \
      ((a) > (b) ? (a) : (b))

    #define MAX3(a, b, c) \
      ((a) > (b) ? ((a) > (c) ? (a) : (c)) : ((b) > (c) ? (b) : (c)))


    // Aligned.
    a[i][j] = GET_SCORE(i, j) + MAX3(a[i - 1][j - 1],
                                     b[i - 1][j - 1],
                                     c[i - 1][j - 1]);

    // Gap.
    b[i][j] = MAX2(a[i - 1][j] + schema->gapOpen,
                   b[i - 1][j] + schema->gapExtend);

    // Gap.
    c[i][j] = MAX2(a[i][j - 1] + schema->gapOpen,
                   c[i][j - 1] + schema->gapExtend);

    #undef MAX3
    #undef MAX2
  }

  #undef GET_SCORE
  #undef GET_CELL_VALUE
  #undef GET_AFFINE_GAP

  return a[lhsSize][rhsSize];
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[128] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  static const ScoringSchema schema =
  {
    +3, // Match.
    -3, // Mismatch.
    -8, // Gap opening.
    -1  // Gap extension.
  };

  for(char lineBuffer[1024] = "";
    fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    const char* const lhs = lineBuffer;

    char* rhs = strchr(lhs, '|');

    assert(rhs && (*(rhs - 1) == ' ') && (*(rhs + 1) == ' '));

    // Terminate LHS.
    *(rhs - 1) = '\0';

    // Jump to RHS's head.
    rhs += 2;

    // Trim RHS.
    {
      char* const garbage = strchr(rhs, '\n');

      if(garbage) *garbage = '\0';
    }

    // As per the problem statement.
    assert((strlen(lhs) <= 400) && (strlen(rhs) <= 400));

    printf("%hd\n", getAlignmentScore(lhs, rhs, &schema));
  }

  // The CRT takes care of cleanup.
}
