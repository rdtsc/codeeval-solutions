#include <assert.h>
#include <stddef.h>
#include <stdio.h>

/*
  0. Supplied Key: BHISOECRTMGWYVALUZDNFJKPQX

  1. Interleave Alphabet: BaHbIcSdOeEfCgRhTiMjGkWlYmVnAoLpUqZrDsNtFuJvKwPxQyXz
                           ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^

  2. a) Transform: /(\w)(\w)/ -> "$1 $2\n"
     b) Sort lines
     c) Transfrom: /\w\s(\w)/ -> "\U$1"

     Transform A   ->   Transform B   ->   Transform C
     -----------        -----------        -----------
         B a                A o                 O
         H b                B a                 A
         I c                C g                 G
         S d                D s                 S
         O e                E f                 F
         E f                F u                 U
         C g                G k                 K
         R h                H b                 B
         T i                I c                 C
         M j                J v                 V
         G k                K w                 W
         W l                L p                 P
         Y m                M j                 J
         V n                N t                 T
         A o                O e                 E
         L p                P x                 X
         U q                Q y                 Y
         Z r                R h                 H
         D s                S d                 D
         N t                T i                 I
         F u                U q                 Q
         J v                V n                 N
         K w                W l                 L
         P x                X z                 Z
         Q y                Y m                 M
         X z                Z r                 R

  3. Transform: /\n/ -> "" = "OAGSFUKBCVWPJTEXYHDIQNLZMR"
*/

int main(void)
{
  const char* const key = "OAGSFUKBCVWPJTEXYHDIQNLZMR";

  char message[] = "012222 1114142503 0313012513 03141418192"
                   "102 0113 2419182119021713 06131715070119";

  char* write = message;

  // Indices are always two digits wide.
  for(const char* c = message; *c; c += 2)
  {
    // Inject a space and don't decode it.
    if(*c == ' ') *write++ = *c++;

    // First and second digits of the current chunk in the message.
    const size_t index = (*c - '0') * 10 + (*(c + 1) - '0');

    assert(index <= ('Z' - 'A'));

    *write++ = key[index];
  }

  // Decoded data is always shorter than the source.
  *write = '\0';

  puts(message);
}
