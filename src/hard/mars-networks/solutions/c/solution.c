#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#ifndef __STDC_IEC_559__
#error
#endif

typedef struct
{
  int x, y;
} Probe;

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

  for(char lineBuffer[2560] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    // At most 250 nodes, as per the problem statement.
    Probe probes[250] = {{0}};
    size_t probeCount = 0;

    // Patch probe positions. Yields a small gain in performance over sscanf
    // and friends in CE's environment due to naive parsing rules and lack of
    // locale awareness on our part.
    {
      int value = 0, * field = &probes->x;

      for(const char* c = lineBuffer;; ++c)
      {
        if(*c >= '0' && *c <= '9') value = (value * 10) + (*c - '0');

        else if(*c == ',' || *c == ' ' || *c == '\n' || !*c)
        {
          assert(probeCount < (sizeof probes / sizeof *probes));

          *field = value;
          value  = 0;

          if(*c == ' ') field = &probes[probeCount].x;

          else if(*c == ',') field = &probes[probeCount++].y;

          else if(*c == '\n' || !*c) break;
        }
      }
    }

    // As per the problem statement.
    assert(probeCount >= 3 && probeCount <= 250);

    // VLAs yield a slightly better memory score on CodeEval than straight
    // [m,c]alloc. Shouldn't rely on this in C11 and onwards, though.
    float distances[probeCount][probeCount];
    memset(distances, 0, sizeof distances);

    // Record probe distances relative to each other.
    for(size_t i = (0 + 0); i < probeCount; ++i)
    for(size_t j = (i + 1); j < probeCount; ++j)
    {
      const float distance = sqrt(pow(probes[i].x - probes[j].x, 2) +
                                  pow(probes[i].y - probes[j].y, 2));

      distances[i][j] = distance;
      distances[j][i] = distance;
    }

    bool used[probeCount];
    size_t indices[probeCount];
    float values[probeCount];

    memset(used, false, sizeof used);
    memset(indices, 0, sizeof indices);

    for(size_t i = 0; i < probeCount; ++i)
    {
      values[i] = FLT_MAX;
    }

    for(size_t i = 0; i < probeCount; ++i)
    {
      size_t j = 0;

      // Find the next best unused node. The upper bound is pretty small so
      // dumb linear search should be fast enough.
      {
        float value = FLT_MAX;

        for(size_t k = 0; k < probeCount; ++k) if(!used[k])
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
      for(size_t k = 0; k < probeCount; ++k) if(!used[k])
      {
        const float value = distances[j][k];

        if(value < values[k])
        {
          values[k]  = value;
          indices[k] = j;
        }
      }
    }

    float pathWeight = 0;

    // Tally path sum.
    for(size_t i = 0; i < probeCount; ++i)
    {
      pathWeight += distances[indices[i]][i];
    }

    printf("%u\n", (unsigned)ceil(pathWeight));
  }

  // The CRT takes care of cleanup.
}
