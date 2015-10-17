#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

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

static bool isCircuit(const size_t width,
                      bool connections[width][width],
                      const size_t offset,
                      const size_t vertexCount,
                      ...)
{
  // Sanity check. Unrestricted VLAs can lead to UB if allocation fails.
  assert((vertexCount >= 2) && (vertexCount <= 128) && !(vertexCount & 1));

  // VLAs yield a slightly better memory score on CodeEval than straight
  // [m,c]alloc. Shouldn't rely on this in C11 and onwards, though.
  size_t vertices[vertexCount];
  memset(vertices, 0, sizeof vertices);

  // Fetch vertex indices.
  {
    va_list pack;
    va_start(pack, vertexCount);

    for(size_t i = 0; i < vertexCount;)
    {
      vertices[i++] = (va_arg(pack, int) + offset - 1);
      vertices[i++] = (va_arg(pack, int) + offset - 1);
    }

    va_end(pack);
  }

  // Attempt to walk the circuit.
  for(size_t i = 0; i < (vertexCount - 1); ++i)
  {
    const size_t x = (vertices[i + 0]),
                 y = (vertices[i + 1]);

    assert(x < width);
    assert(y < width);

    if(!connections[x][y]) return false;
  }

  // The last vertex should link up with the head to complete the circuit.
  return connections[*vertices][vertices[vertexCount - 1]];
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

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    #define MAP_WIDTH 5
    #define MAP_AREA (MAP_WIDTH * MAP_WIDTH)

    bool connections[MAP_AREA][MAP_AREA] = {{false}};

    // Unserialize connections.
    for(const char* cursor = lineBuffer; *cursor;)
    {
      size_t lhs = extractUint(&cursor),
             rhs = extractUint(&cursor);

      assert(lhs != rhs);
      assert((lhs >= 1) && (lhs <= MAP_AREA));
      assert((rhs >= 1) && (rhs <= MAP_AREA));

      // Zero-based indices.
      --lhs; --rhs;

      connections[lhs][rhs] = true;
      connections[rhs][lhs] = true;
    }

    unsigned squareCount = 0;

    #define COUNT(...) (sizeof (int[]){__VA_ARGS__} / sizeof(int))

    #define IS_CIRCUIT(Pad, ...) \
      isCircuit(MAP_AREA, connections, (Pad), COUNT(__VA_ARGS__), __VA_ARGS__)

    // Unit squares based on the diagram in the problem statement.
    #define UNIT_1 1, 2, 7, 6
    #define UNIT_2 1, 2, 3, 8, 13, 12, 11, 6
    #define UNIT_3 1, 2, 3, 4, 9, 14, 19, 18, 17, 16, 11, 6
    #define UNIT_4 1, 2, 3, 4, 5, 10, 15, 20, 25, 24, 23, 22, 21, 16, 11, 6

    // 1x1.
    for(size_t x = 0; x < (MAP_WIDTH - 1); ++x)
    for(size_t y = 0; y < (MAP_WIDTH * 4); y += MAP_WIDTH)
    {
      if(IS_CIRCUIT((x + y), UNIT_1)) ++squareCount;
    }

    // 2x2.
    for(size_t x = 0; x < (MAP_WIDTH - 2); ++x)
    for(size_t y = 0; y < (MAP_WIDTH * 3); y += MAP_WIDTH)
    {
      if(IS_CIRCUIT((x + y), UNIT_2)) ++squareCount;
    }

    // 3x3.
    for(size_t x = 0; x < (MAP_WIDTH - 3); ++x)
    for(size_t y = 0; y < (MAP_WIDTH * 2); y += MAP_WIDTH)
    {
      if(IS_CIRCUIT((x + y), UNIT_3)) ++squareCount;
    }

    // 4x4.
    if(IS_CIRCUIT(0, UNIT_4)) ++squareCount;

    #undef UNIT_4
    #undef UNIT_3
    #undef UNIT_2
    #undef UNIT_1
    #undef IS_CIRCUIT
    #undef COUNT
    #undef MAP_AREA
    #undef MAP_WIDTH

    printf("%u\n", squareCount);
  }

  // The CRT takes care of cleanup.
}
