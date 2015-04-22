#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[4096] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[256] = "", haystack[256] = "", outputBuffer[512] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    typedef struct
    {
      char* data;
      size_t length;
    } String;

    typedef struct
    {
      String subject;
      String replacement;
    } Replacement;

    Replacement replacements[64] = {{{0}, {0}}};
    size_t replacementCount = 0;

    // Parse input line.
    {
      char* c = lineBuffer;

      // Isolate the source string and replacement operations.
      {
        while(*c && *c != ';') ++c;

        *c++ = '\0';

        memcpy(haystack, lineBuffer, c - lineBuffer);
      }

      #define GET_CHUNK() strtok(NULL, ",\n")

      // Get replacement operations.
      for(char* subject = strtok(c, ",\n"), * replacement = GET_CHUNK();
          subject; subject = GET_CHUNK(), replacement = GET_CHUNK())
      {
        assert(replacementCount < (sizeof replacements / sizeof *replacements));

        Replacement* const record = &replacements[replacementCount];

        record->subject.data = subject;
        record->subject.length = strlen(subject);

        record->replacement.data = replacement;
        record->replacement.length = strlen(replacement);

        ++replacementCount;
      }

      #undef GET_CHUNK

      assert(replacementCount);
    }

    // We're dealing with a binary sequence. Indices into the replacement
    // map can start from 2 upwards. If required, patch the ASCII offsets to
    // accommodate a larger number of supported replacement operations.
    static const char baseReplacementKey = '2';

    // Construct a (unique) replacement mask.
    for(size_t i = 0; i < replacementCount; ++i)
    {
      Replacement* const record = &replacements[i];

      // Replace all occurrences of the needle in the bookkeeping haystack
      // with its corresponding index in the replacement map.
      for(char* haystackPos = strstr(haystack, record->subject.data);
          haystackPos; haystackPos = strstr(haystack, record->subject.data))
      {
        memmove(haystackPos + 1,
                haystackPos + record->subject.length,
                strlen(haystack) - record->subject.length + 1);

        *haystackPos = (baseReplacementKey + i);
      }
    }

    // Construct the resultant string based on the replacement mask.
    {
      char* out = outputBuffer;

      for(const char* c = haystack; *c; ++c) switch(*c)
      {
        // Part of the original sequence.
        case '0': case '1': *out++ = *c; break;

        // Index into the replacement map.
        default:
        {
          const size_t key = (*c - baseReplacementKey);
          const String* const replacement = &replacements[key].replacement;

          for(size_t i = 0; i < replacement->length; ++i)
          {
            *out++ = replacement->data[i];
          }
        }
      }

      *out = '\0';
    }

    puts(outputBuffer);
  }

  // The CRT takes care of cleanup.
}
