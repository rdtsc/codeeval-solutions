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

typedef struct
{
  unsigned sourceId, targetId;
} Path;

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

static unsigned pathGetLength(const Path* const path,
                              const Edge* const edges,
                              const size_t edgeCount)
{
  assert(path && edges && edgeCount);

  const size_t matrixWidth = getMaxVertexId(edges, edgeCount);

  // Sanity check. Unrestricted VLAs can lead to UB if allocation fails.
  assert(matrixWidth <= 1e3);

  // VLAs yield a slightly better memory score on CodeEval than straight
  // [m,c]alloc. Shouldn't rely on this in C11 and onwards, though.
  unsigned matrix[matrixWidth][matrixWidth];
  memset(matrix, 0, sizeof matrix);

  // Fill the adjacency matrix.
  for(size_t i = 0; i < edgeCount; ++i)
  {
    const Edge* const edge = &edges[i];

    matrix[edge->u - 1][edge->v - 1] = edge->w;
    matrix[edge->v - 1][edge->u - 1] = edge->w;
  }

  // Ditto regarding VLAs.
  unsigned distances[matrixWidth];
  bool used[matrixWidth];

  for(size_t i = 0; i < matrixWidth; ++i)
  {
    distances[i] = UINT_MAX;
    used[i] = false;
  }

  #define FOR_UNUSED(Name) \
    for(size_t (Name) = 0; (Name) < matrixWidth; ++(Name)) if(!used[(Name)])

  distances[path->sourceId - 1] = 0;

  // Dijkstra's algorithm.
  for(size_t cellIndex = 0; cellIndex < matrixWidth; ++cellIndex)
  {
    size_t i = 0;

    {
      unsigned distance = UINT_MAX;

      FOR_UNUSED(j) if(distances[j] <= distance)
      {
        i = j;
        distance = distances[j];
      }

      used[i] = true;
    }

    FOR_UNUSED(j) if(matrix[i][j] && (distances[i] != UINT_MAX))
    {
      const unsigned distance = (matrix[i][j] + distances[i]);

      if(distance < distances[j]) distances[j] = distance;
    }
  }

  #undef FOR_UNUSED

  return distances[path->targetId - 1];
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

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    Edge edges[32] = {{0}};
    size_t edgeCount = 0;

    // Unserialize graph.
    for(const char* cursor = lineBuffer; *cursor;)
    {
      assert(edgeCount < (sizeof edges / sizeof *edges));

      edges[edgeCount++] = (Edge)
      {
        .u = extractUint(&cursor),
        .v = extractUint(&cursor),
        .w = extractUint(&cursor),
      };
    }

    assert(edgeCount > 1);

    // Account for the the trailing source/destination edge pair.
    --edgeCount;

    // Source/destination edge pair will have no weight.
    assert(!edges[edgeCount].w);

    const Path path =
    {
      .sourceId = edges[edgeCount].u,
      .targetId = edges[edgeCount].v
    };

    assert((path.sourceId >= 1) && (path.sourceId != path.targetId));

    const unsigned pathLength = pathGetLength(&path, edges, edgeCount);

    if(pathLength != UINT_MAX) printf("%u\n", pathLength);

    else puts("False");
  }

  // The CRT takes care of cleanup.
}
