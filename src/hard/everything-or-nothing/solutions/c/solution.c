#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef enum
{
  PERMISSION_GRANT = 1,
  PERMISSION_WRITE = 2,
  PERMISSION_READ  = 4
} Permission;

static Permission getPermissionValue(const char* const string)
{
  assert(string && *string);

  switch(*string)
  {
    case 'g': return PERMISSION_GRANT;
    case 'w': return PERMISSION_WRITE;
    case 'r': return PERMISSION_READ;
  }

  assert(false);

  return 0;
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

  // Base access control list as per the problem statement.
  static const unsigned char defaultAcl[][3] =
  {
    /*
      0: none
      1: grant
      2: write
      3: write, grant
      4: read
      5: read, grant
      6: read, write
      7: read, write, grant
    */

    //           file_1  file_2  file_3
    /* user_1 */  {7,      3,      0},
    /* user_2 */  {6,      2,      4},
    /* user_3 */  {5,      1,      5},
    /* user_4 */  {3,      7,      1},
    /* user_5 */  {6,      0,      2},
    /* user_6 */  {4,      2,      6}
  };

  for(char lineBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    unsigned char acl[sizeof  defaultAcl / sizeof  *defaultAcl]
                     [sizeof *defaultAcl / sizeof **defaultAcl] = {""};

    // Revert to stock ACL for each test case.
    memcpy(acl, defaultAcl, sizeof defaultAcl);

    bool isValidActionChain = true;

    for(const char* chunk = strtok(lineBuffer, " ");
        chunk; chunk = strtok(NULL, " "))
    {
      /*
        Representing either:
        - "userId=>fileId=>rule", or;
        - "userId=>fileId=>rule=>newRule=>subjectId"
      */

      unsigned userId    = 0,
               fileId    = 0,
               rule      = 0,
               newRule   = 0,
               subjectId = 0;

      char ruleBuffer[8] = "", newRuleBuffer[8] = "";

      #define ARGUMENT_PACK \
        &userId, &fileId, ruleBuffer, newRuleBuffer, &subjectId

      static const char* const scanFormat =
        "user_%u=>file_%u=>%7[^=]=>%7[^=]=>user_%u";

      // Should be 3 or 5, really.
      if(sscanf(chunk, scanFormat, ARGUMENT_PACK) < 3)
      {
        assert(false);
      }

      rule = getPermissionValue(ruleBuffer);

      // Trailing data is only present if this is a grant operation.
      if(rule == PERMISSION_GRANT)
      {
        newRule = getPermissionValue(newRuleBuffer);
      }

      #undef ARGUMENT_PACK

      // IDs are 1-based in the input.
      const unsigned char ownerRights = acl[userId - 1][fileId - 1];

      // Self-administer supplied permission.
      if(!subjectId)
      {
        if(!(ownerRights & rule))
        {
          isValidActionChain = false;
          break;
        }

        // Already have this permission, so NOP.
        continue;
      }

      // Insufficient rights.
      if(!(ownerRights & PERMISSION_GRANT))
      {
        isValidActionChain = false;
        break;
      }

      // Apply supplied permission to another user.
      acl[subjectId - 1][fileId - 1] |= newRule;
    }

    puts(isValidActionChain ? "True" : "False");
  }

  // The CRT takes care of cleanup.
}
