#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  double x, y;
} Point2d;

typedef struct
{
  unsigned id;
  Point2d a, b;
} Bridge;

typedef struct
{
  size_t size;
  unsigned data[32];
} ResultPool;

static inline int strictWeakOrdering(const void* const lhs,
                                     const void* const rhs)
{
  assert(lhs && rhs);

  const unsigned x = *(unsigned*)lhs,
                 y = *(unsigned*)rhs;

  return (x < y) ? (-1) : (x > y);
}

static inline bool hasIntersection(const Bridge* const lhs,
                                   const Bridge* const rhs)
{
  assert(lhs && rhs);

  #define IS_COUNTERCLOCKWISE(A, B, C) \
    ((((C).y - (A).y) * ((B).x - (A).x)) > (((B).y - (A).y) * ((C).x - (A).x)))

  #define IS_INTERSECTION(A, B, C, D, E, F) \
    (IS_COUNTERCLOCKWISE(A, B, C) != IS_COUNTERCLOCKWISE(D, E, F))

  const Point2d a = lhs->a, b = lhs->b,
                c = rhs->a, d = rhs->b;

  return IS_INTERSECTION(a, c, d, b, c, d) &&
         IS_INTERSECTION(a, b, c, a, b, d);

  #undef IS_INTERSECTION
  #undef IS_COUNTERCLOCKWISE
}

static void dumpOptimalBridgeSet(const Bridge* const bridges,
                                 const size_t bridgeCount,
                                 FILE* const outputStream)
{
  assert(bridges && bridgeCount && outputStream && !ferror(outputStream));

  // Sanity check. Unrestricted VLAs can lead to UB if allocation fails.
  assert(bridgeCount < 32);

  // VLAs yield a slightly better memory score on CodeEval than straight
  // [m,c]alloc. Shouldn't rely on this in C11 and onwards, though.
  bool intersections[bridgeCount + 1][bridgeCount + 1];
  memset(intersections, 0, sizeof intersections);

  // Bridge IDs appear to start from 1 and are sequential.
  for(size_t i = 0; i < bridgeCount; ++i)
  for(size_t j = 0; j < bridgeCount; ++j)
  {
    if(hasIntersection(&bridges[i], &bridges[j]))
    {
      intersections[bridges[i].id][bridges[j].id] = true;
      intersections[bridges[j].id][bridges[i].id] = true;
    }
  }

  ResultPool results = {0};

  // Find the largest combination of non-intersecting bridges.
  {
    ResultPool layout = {0};

    // Ditto regarding VLAs.
    size_t select[bridgeCount];
    memset(select, 0, sizeof select);

    for(size_t i = 1; i <= bridgeCount; ++i)
    {
      for(size_t j = 0; j < i; ++j) select[j] = j;

      while(select[i - 1] < bridgeCount)
      {
        layout.size = 0;

        for(size_t j = 0; j < i; ++j)
        {
          bool canLayBridge = true;

          // Candidates must not intersect with anything in the current layout.
          for(size_t k = 0; k < layout.size; ++k)
          {
            const unsigned thisId = layout.data[k],
                           newId  = bridges[select[j]].id;

            if(intersections[thisId][newId])
            {
              canLayBridge = false;
              break;
            }
          }

          if(canLayBridge)
          {
            assert(layout.size < (sizeof layout.data / sizeof *layout.data));

            layout.data[layout.size++] = (select[j] + 1);
          }

          else goto nextCombination;
        }

        if(layout.size > results.size) results = layout;

        nextCombination:;

        int k = (i - 1);

        for(;(k != 0) && ((bridgeCount - i + k) == select[k]); --k);

        ++select[k];

        for(size_t j = (k + 1); j < i; ++j) select[j] = select[j - 1] + 1;
      }
    }
  }

  // Bridge IDs should be in ascending order, as per the problem statement.
  qsort(results.data, results.size, sizeof *results.data, strictWeakOrdering);

  for(size_t i = 0; i < results.size; ++i)
  {
    fprintf(outputStream, "%u\n", results.data[i]);
  }
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

  Bridge bridges[32] = {{0}};
  size_t bridgeCount = 0;

  // Fetch bridge pool.
  {
    static const char* const scanFormat = "%u: ([%lf, %lf], [%lf, %lf])";

    Bridge* bridge = bridges;

    #define ARGUMENT_PACK \
      &bridge->id, &bridge->a.x, &bridge->a.y, &bridge->b.x, &bridge->b.y

    while(fscanf(inputStream, scanFormat, ARGUMENT_PACK) == 5)
    {
      assert(bridgeCount < (sizeof bridges / sizeof *bridges));

      bridge = &bridges[++bridgeCount];
    }

    #undef ARGUMENT_PACK
  }

  dumpOptimalBridgeSet(bridges, bridgeCount, stdout);

  // The CRT takes care of cleanup.
}
