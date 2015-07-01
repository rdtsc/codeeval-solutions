#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
  unsigned port;
  char scheme[32];
  char host[256];
  char path[512];
} Uri;

static void uriNormalize(Uri* const uri)
{
  assert(uri);

  #define TO_LOWER(Target) \
    for(char* c = (Target); *c; ++c) *c = tolower(*c);

  TO_LOWER(uri->scheme);
  TO_LOWER(uri->host);

  #undef TO_LOWER

  char path[sizeof uri->path] = "", * out = path;

  for(const char* c = uri->path; *c; ++c)
  {
    // URL-encoded character.
    if(*c == '%' && isxdigit(*(c + 1)))
    {
      unsigned octet = '\0';

      if(sscanf(++c, "%2x", &octet) != 1) assert(false);

      assert(isprint(octet));

      *out++ = (char)octet;
      ++c;
    }

    else *out++ = *c;
  }

  memcpy(uri->path, path, sizeof path);
}

static bool uriExplode(const char* const uri, Uri* const bucket)
{
  assert(uri && *uri && bucket);

  memset(bucket, 0, sizeof *bucket);

  bool isSuccess = false;

  unsigned* const port = &bucket->port;

  char* const scheme = bucket->scheme,
      * const host   = bucket->host,
      * const path   = bucket->path;

  // Explicit port and path.
  if(sscanf(uri, "%31[^:]://%255[^:]:%u/%511s", scheme, host, port, path) == 4)
  {
    assert(bucket->port < 65536U);

    isSuccess = true;
  }

  // Implicit port and explicit path.
  else if(sscanf(uri, "%31[^:]://%255[^/]/%511s", scheme, host, path) == 3)
  {
    bucket->port = 80;

    isSuccess = true;
  }

  // Explicit port and missing path.
  else if(sscanf(uri, "%31[^:]://%255[^:]%u", scheme, host, port) == 3)
  {
    isSuccess = true;
  }

  // Implicit port and missing path.
  else if(sscanf(uri, "%31[^:]://%255s", scheme, host) == 2)
  {
    isSuccess = true;
  }

  if(isSuccess) uriNormalize(bucket);

  return isSuccess;
}

static bool uriIsEqual(const Uri* const lhs, const Uri* const rhs)
{
  assert(lhs && rhs);

  const bool isSamePort = (lhs->port == rhs->port);

  #define IS_SAME(Component) (!strcmp(lhs->Component, rhs->Component))

  return isSamePort      &&
         IS_SAME(scheme) &&
         IS_SAME(host)   &&
         IS_SAME(path);

  #undef IS_SAME
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
    char* delimiter = strchr(lineBuffer, ';');

    assert(delimiter);

    *delimiter++ = '\0';

    Uri uri[2] = {{0}};

    if(!uriExplode(lineBuffer, &uri[0]) || !uriExplode(delimiter, &uri[1]))
    {
      assert(false);
    }

    puts(uriIsEqual(&uri[0], &uri[1]) ? "True" : "False");
  }

  // The CRT takes care of cleanup.
}
