#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct
{
  unsigned id, value, weight;
} Item;

typedef struct
{
  uint_least32_t items;
  unsigned value, weight;
} Package;

static inline void packageAddItem(Package* const package,
                                  const Item* const item)
{
  assert(package && item && item->id);

  package->items  |= (1 << (item->id - 1));
  package->value  += item->value;
  package->weight += item->weight;
}

static void doGetBestPackage(const Item* const items,
                             const size_t itemCount,
                             const size_t itemIndex,
                             const unsigned boxCapacity,
                             Package* const result)
{
  assert(items && itemCount && boxCapacity && result);

  if(itemIndex == itemCount) return;

  const Item* const item = &items[itemIndex];
  const size_t nextItemIndex = (itemIndex + 1);

  if(boxCapacity < item->weight)
  {
    doGetBestPackage(items, itemCount, nextItemIndex, boxCapacity, result);
    return;
  }

  Package take = {0}, pass = {0};

  // Take the item.
  {
    const size_t newBoxCapacity = (boxCapacity - item->weight);

    doGetBestPackage(items, itemCount, nextItemIndex, newBoxCapacity, &take);

    packageAddItem(&take, item);
  }

  // Pass on the item.
  doGetBestPackage(items, itemCount, nextItemIndex, boxCapacity, &pass);

  // No difference in value.
  if(take.value == pass.value)
  {
    // Keep the item with the smaller weight.
    *result = ((take.weight < pass.weight) ? take : pass);
  }

  // Keep the item with the larger value.
  else *result = ((take.value > pass.value) ? take : pass);
}

static void getBestPackage(const Item* const items,
                           const size_t itemCount,
                           const unsigned boxCapacity,
                           Package* const result)
{
  assert(items && itemCount && boxCapacity && result);

  doGetBestPackage(items, itemCount, 0, boxCapacity, result);
}

static void packageDumpContents(const Package* const package,
                                FILE* const outputStream)
{
  assert(package && outputStream && !ferror(outputStream));

  // No item IDs.
  if(!package->items)
  {
    fputs("-\n", outputStream);
    return;
  }

  static const size_t maskBitCount = (sizeof(uint_least32_t) * CHAR_BIT);

  size_t i = 0;

  // Advance to first item ID.
  while(!(package->items & (1 << i))) ++i;

  fprintf(outputStream, "%zu", (i + 1));

  // Dump any remaining item IDs.
  for(++i; i < maskBitCount; ++i) if(package->items & (1 << i))
  {
    fprintf(outputStream, ",%zu", (i + 1));
  }

  fputc('\n', outputStream);
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

  for(char lineBuffer[512] = "", * cursor = lineBuffer;
      fgets(lineBuffer, sizeof lineBuffer, inputStream);
      cursor = lineBuffer)
  {
    int bytesRead = 0;
    unsigned boxCapacity = 0;

    if(sscanf(lineBuffer, "%u", &boxCapacity) == 1)
    {
      boxCapacity *= 100;

      // Advance to the head of the item list.
      while(*cursor && *cursor++ != ':');

      assert(*cursor);

      Item items[32] = {{0}};
      size_t itemCount = 0;

      // Fetch item pool.
      {
        unsigned id = 0;
        double value = 0, weight = 0;

        static const char* const itemFormat = "%*[^(](%u,%lf,$%lf)%n";

        for(int bytesRead = 0;
            sscanf(cursor, itemFormat, &id, &weight, &value, &bytesRead) == 3;
            cursor += bytesRead)
        {
          // As per the problem statement.
          assert(id < 16);

          assert(itemCount < (sizeof items / sizeof *items));

          Item* const item = &items[itemCount++];

          item->id     = id;
          item->weight = (weight * 100);
          item->value  = (value  * 100);
        }

        // As per the problem statement.
        assert(itemCount < 16);
      }

      Package optimalPackage = {0};

      getBestPackage(items, itemCount, boxCapacity, &optimalPackage);
      packageDumpContents(&optimalPackage, stdout);
    }
  }

  // The CRT takes care of cleanup.
}
