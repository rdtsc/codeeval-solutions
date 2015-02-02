#include <inttypes.h>
#include <stdio.h>

int main(void)
{
  typedef uint_least16_t value_type;

  union
  {
    unsigned char payload[sizeof(value_type)];
    value_type value;
  } const probe = {{1}};

  /*
    Assuming that `uint_least16_t` corresponds to a type that's 16 bits wide,
    the payload is initialized with the following bytes: 01 00

    The above will get interpreted as 0x0100 on a Big-endian architecture
    and as 0x0001 on a Little-endian architecture. PDP-endianness is also
    possible, although the problem statement doesn't require its detection.
  */

  puts(probe.value == 1 ? "LittleEndian" : "BigEndian");
}
