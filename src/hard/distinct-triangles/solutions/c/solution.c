#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef unsigned char matrix_value_type;
typedef matrix_value_type matrix_type[100][100];

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

static size_t getCubedAdjacencyTrace(matrix_type edges, const size_t width)
{
  struct {matrix_type first, second;} scratch = {.first = {{0}}};

  memcpy(scratch.first, edges, sizeof(matrix_type));

  // Cube the adjacency matrix.
  for(size_t i = 0; i < 2; ++i)
  {
    for(size_t y = 0; y < width; ++y)
    for(size_t x = 0; x < width; ++x)
    {
      matrix_value_type cell = 0;

      for(size_t j = 0; j < width; ++j)
      {
        cell += scratch.first[y][j] * edges[j][x];
      }

      scratch.second[y][x] = cell;
    }

    memcpy(scratch.first, scratch.second, sizeof(matrix_type));
  }

  size_t result = 0;

  // Trace the cubed adjacency matrix.
  for(size_t i = 0; i < width; ++i)
  {
    result += scratch.first[i][i];
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

  for(char lineBuffer[1024] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    const char* cursor = lineBuffer;

    const size_t vertexCount = extractUint(&cursor),
                   edgeCount = extractUint(&cursor);

    assert(vertexCount >= 001 && edgeCount >= 001);
    assert(vertexCount <= 100 && edgeCount <= 100);

    matrix_type edges = {{0}};

    for(size_t i = 0; i < edgeCount; ++i)
    {
      struct {unsigned a, b;} const edge =
      {
        extractUint(&cursor),
        extractUint(&cursor)
      };

      edges[edge.a][edge.b] = 1;
      edges[edge.b][edge.a] = 1;
    }

    // Looking for paths of L <- 3; ignoring duplicates.
    printf("%zu\n", getCubedAdjacencyTrace(edges, vertexCount) / 6);
  }

  // The CRT takes care of cleanup.
}
