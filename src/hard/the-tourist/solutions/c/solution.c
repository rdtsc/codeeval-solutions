#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
  unsigned u, v, w;
} Edge;

// Yields a small gain in performance over sscanf and friends in CE's
// environment due to naive parsing rules and lack of locale awareness
// on our part.
static unsigned extractUint(const char** string)
{
  assert(string && *string);

  unsigned result = 0;
  const char* c = *string;

  #define IS_PAYLOAD() (*c >= '0' && *c <= '9')

  // Eat anything that's not a digit.
  while(*c && !IS_PAYLOAD()) ++c;

  for(;*c; ++c)
  {
    if(!IS_PAYLOAD())
    {
      ++c;
      break;
    }

    else result = (result * 10) + (*c - '0');
  }

  #undef IS_PAYLOAD

  *string = c;

  return result;
}

static unsigned getMaxVertexId(const Edge* const edges, const size_t edgeCount)
{
  assert(edges && edgeCount);

  #define MAX(A, B) ((A) > (B) ? (A) : (B))

  unsigned result = 0;

  for(size_t i = 0; i < edgeCount; ++i)
  {
    const unsigned peak = MAX(edges[i].u, edges[i].v);

    if(peak > result) result = peak;
  }

  return result;

  #undef MAX
}

// Reusing challenge 164's solution.
static unsigned getMstLength(const Edge* const edges, const size_t edgeCount)
{
  assert(edges && edgeCount);

  const unsigned tableWidth = getMaxVertexId(edges, edgeCount);

  // As per the problem statement.
  assert((tableWidth >= 3) && (tableWidth <= 100));

  // VLAs yield a slightly better memory score on CodeEval than straight
  // [m,c]alloc. Shouldn't rely on this in C11 and onwards, though.
  unsigned distances[tableWidth][tableWidth];
  memset(distances, 0, sizeof distances);

  // Fill the distance table.
  for(size_t i = 0; i < edgeCount; ++i)
  {
    const Edge* const edge = &edges[i];

    distances[edge->u - 1][edge->v - 1] = edge->w;
    distances[edge->v - 1][edge->u - 1] = edge->w;
  }

  bool used[tableWidth];
  size_t indices[tableWidth];
  unsigned values[tableWidth];

  memset(used, false, sizeof used);
  memset(indices, 0, sizeof indices);

  for(size_t i = 0; i < tableWidth; ++i)
  {
    values[i] = UINT_MAX;
  }

  for(size_t i = 0; i < tableWidth; ++i)
  {
    size_t j = 0;

    // Find the next best unused node. The upper bound is pretty small so
    // dumb linear search should be fast enough.
    {
      unsigned value = UINT_MAX;

      for(size_t k = 0; k < tableWidth; ++k) if(!used[k])
      {
        if(values[k] < value)
        {
          j = k;
          value = values[k];
        }
      }

      used[j] = true;
    }

    // Record path.
    for(size_t k = 0; k < tableWidth; ++k) if(!used[k])
    {
      const unsigned value = distances[j][k];

      if(value && value < values[k])
      {
        values[k]  = value;
        indices[k] = j;
      }
    }
  }

  // Presence of unvisited vertices means the "must visit all cities"
  // constraint can't be satisfied.
  for(size_t i = 0; i < tableWidth; ++i) if(!used[i]) return 0;

  unsigned result = 0;

  // Tally path sum.
  for(size_t i = 0; i < tableWidth; ++i)
  {
    result += distances[indices[i]][i];
  }

  return result;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[256] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[8192] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    Edge edges[640] = {{0}};
    size_t edgeCount = 0;

    // Unserialize graph.
    for(const char* cursor = lineBuffer; *cursor;)
    {
      assert(edgeCount < (sizeof edges / sizeof *edges));

      Edge* const edge = &edges[edgeCount++];

      edge->u = extractUint(&cursor); // Vertex A.
      edge->v = extractUint(&cursor); // Vertex B.
      edge->w = extractUint(&cursor); // Edge weight.
    }

    assert(edgeCount);

    const unsigned pathLength = getMstLength(edges, edgeCount);

    if(pathLength) printf("%u\n", pathLength);

    else puts("False");
  }

  // The CRT takes care of cleanup.
}
