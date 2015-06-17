#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  size_t friendCount;
  size_t groupCount;
  char friends[62][16];
  char groups[16][32];
  char name[16];
} User;

typedef User UserPool[200];
typedef char NameIndex[200][16];
typedef char GroupIndex[16][32];

static inline int lexicographicalOrdering(const void* const lhs,
                                          const void* const rhs)
{
  assert(lhs && rhs);

  return strcmp((const char*)lhs, (const char*)rhs);
}

static User* getUserByName(UserPool users,
                           NameIndex nameIndex,
                           const size_t userCount,
                           const char* const name)
{
  assert(users && nameIndex && userCount && name && *name);

  const size_t nameWidth = (sizeof *nameIndex);

  typedef int(* Comparator)(const void*, const void*);

  const Comparator comparator = (Comparator)strcmp;

  const char* const target =
    bsearch(name, nameIndex, userCount, nameWidth, comparator);

  assert(target);

  return &users[(target - *nameIndex) / nameWidth];
}

static size_t getGroupIndexByName(GroupIndex groupIndex,
                                  const size_t groupCount,
                                  const char* const name)
{
  assert(groupIndex && groupCount && name && *name);

  const size_t groupNameWidth = (sizeof *groupIndex);

  typedef int(* Comparator)(const void*, const void*);

  const Comparator comparator = (Comparator)strcmp;

  const char* const target =
    bsearch(name, groupIndex, groupCount, groupNameWidth, comparator);

  assert(target);

  return ((target - *groupIndex) / groupNameWidth);
}

static size_t hash(const char* string)
{
  // Standard 32-bit FNV-1a constants.
  static const size_t prime = 0x01000193;

  size_t result = 0x811C9DC5;

  while(*string) result = (result * prime) ^ (*string++);

  return result;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[16384] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  UserPool users        = {{00}};
  NameIndex nameIndex   = {{""}};
  GroupIndex groupIndex = {{""}};

  // Should be large enough at prime (2^9 + 9). Collisions are NOT handled.
  char* groupSet[521] = {false};
  const size_t groupSetSize = (sizeof groupSet / sizeof *groupSet);

  size_t userCount  = 0,
         groupCount = 0;

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    assert(userCount < (sizeof users / sizeof *users));

    User* const user = &users[userCount];
    char* cursor = strchr(lineBuffer, ':');

    assert(cursor);

    // Name.
    {
      *cursor++ = '\0';

      const size_t chunkLength = (cursor - lineBuffer - 1);

      memcpy(user->name, lineBuffer, chunkLength);
      memcpy(nameIndex[userCount++], lineBuffer, chunkLength);
    }

    // Friends.
    for(char* chunk = cursor;; ++cursor) if(*cursor == ',' || *cursor == ':')
    {
      const bool isDone = (*cursor == ':');

      *cursor++ = '\0';

      const size_t chunkLength = (cursor - chunk - 1);

      memcpy(user->friends[user->friendCount++], chunk, chunkLength + 1);

      chunk = cursor;

      if(isDone) break;
    }

    // Groups.
    if(*cursor && *cursor != '\n') for(char* chunk = cursor;; ++cursor)
    {
      if(*cursor == ',' || *cursor == '\n' || !*cursor)
      {
        const bool isDone = (*cursor == '\n' || !*cursor);

        *cursor++ = '\0';

        const size_t chunkLength = (cursor - chunk - 1);

        memcpy(user->groups[user->groupCount++], chunk, chunkLength);

        // Store unique groups.
        {
          const size_t groupId = (hash(chunk) % groupSetSize);

          if(!groupSet[groupId])
          {
            memcpy(groupIndex[groupCount], chunk, chunkLength);
            groupSet[groupId] = groupIndex[groupCount++];
          }
        }

        chunk = cursor;

        if(isDone) break;
      }
    }
  }

  // Sort the group index to facilitate binary searching later on.
  qsort(groupIndex, groupCount, sizeof *groupIndex, lexicographicalOrdering);

  for(size_t i = 0; i < userCount; ++i)
  {
    const User* const user = &users[i];
    const size_t friendCount = user->friendCount;
    unsigned groupMembership[sizeof(GroupIndex) / sizeof *groupIndex] = {0};

    // Fill the friend group membership frequency map. Should be using hashes
    // instead of binary search, but don't feel like dealing with collisions
    // right now.
    for(size_t j = 0; j < friendCount; ++j)
    {
      const User* const buddy =
        getUserByName(users, nameIndex, userCount, user->friends[j]);

      assert(buddy);

      for(size_t k = 0; k < buddy->groupCount; ++k)
      {
        const size_t groupId =
          getGroupIndexByName(groupIndex, groupCount, buddy->groups[k]);

        ++groupMembership[groupId];
      }
    }

    // Filter out the groups we already belong to.
    for(size_t j = 0; j < user->groupCount; ++j)
    {
      const size_t groupId =
        getGroupIndexByName(groupIndex, groupCount, user->groups[j]);

      groupMembership[groupId] = 0;
    }

    // Dump suggestions, if any.
    {
      size_t j = 0;

      #define IS_VALID_GROUP() (groupMembership[j] * 2 >= friendCount)
      #define DUMP(String) fputs((String), stdout)

      for(;j < groupCount; ++j) if(IS_VALID_GROUP()) break;

      // Are there any groups to suggest?
      if(j == groupCount) continue;

      DUMP(user->name);
      DUMP(":");
      DUMP(groupIndex[j]);

      for(++j; j < groupCount; ++j) if(IS_VALID_GROUP())
      {
        DUMP(",");
        DUMP(groupIndex[j]);
      }

      DUMP("\n");

      #undef DUMP
      #undef IS_VALID_GROUP
    }
  }

  // The CRT takes care of cleanup.
}
