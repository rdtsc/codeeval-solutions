#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[512] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[8192] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    char* c = lineBuffer;

    unsigned char ipRoute[8] = {0};

    // Fetch the new source/destination addresses.
    for(size_t i = 0; i < sizeof ipRoute; ++c) switch(*c)
    {
      case '.': case ' ': ++i; break;

      default:
      {
        assert(isdigit(*c));

        ipRoute[i] = (ipRoute[i] * 10) + (*c - '0');
      }
    }

    // Since we're only patching a portion of the header, we can reuse the
    // partially formatted input buffer later.
    char* const patchedIpv4Header = c;

    // IPv4 headers are 20 bytes wide. The layout should be available here:
    // http://en.wikipedia.org/wiki/IPv4#Header
    unsigned char ipv4Header[20] = {0};

    // Fetch the header.
    for(size_t i = 0; i < sizeof ipv4Header - sizeof ipRoute; ++i, ++c)
    {
      char* breakpoint = c;

      const unsigned char byte = strtol(c, &breakpoint, 16);

      assert(breakpoint != c);

      ipv4Header[i] = byte;
      c = breakpoint;
    }

    // Discard the provided header checksum.
    ipv4Header[10] = ipv4Header[11] = 0;

    // Inject the new source and destination addresses.
    for(size_t i = 0; i < sizeof ipRoute / 2; ++i)
    {
      ipv4Header[12 + i] = ipRoute[i + 0];
      ipv4Header[16 + i] = ipRoute[i + 4];
    }

    // Construct the header checksum as per:
    // http://en.wikipedia.org/wiki/IPv4_header_checksum
    {
      unsigned sum = 0;

      for(size_t i = 0; i < sizeof ipv4Header; i += 2)
      {
        sum += (ipv4Header[i] << 8) + ipv4Header[i + 1];
      }

      while(sum >> 16) sum = (sum & 0xFFFF) + (sum >> 16);

      sum = ~sum;

      ipv4Header[10] = (sum & 0xFF00) >> 8;
      ipv4Header[11] = (sum & 0x00FF) >> 0;
    }

    // Inject the updated last 10 bytes of the header into the pre-formatted
    // input buffer.
    {
      // Start writing at offset: (10 header bytes) * (2 xdigits + 1 space)
      char* out = patchedIpv4Header + 30;

      for(size_t i = 10; i < sizeof ipv4Header; ++i)
      {
        static const char* const digits = "0123456789abcdef";

        const unsigned value = ipv4Header[i];

        *out++ = digits[(value >> 4) & 0x0F];
        *out++ = digits[(value >> 0) & 0x0F];
        *out++ = ' ';
      }

      *out = '\0';
    }

    puts(patchedIpv4Header);
  }

  // The CRT takes care of cleanup.
}
