#include <iostream>

template<unsigned char... Bytes> struct Decode
{
  static const char message[sizeof...(Bytes) + 1];
};

template<unsigned char... Bytes>
const char Decode<Bytes...>::message[sizeof...(Bytes) + 1] =
{
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

  Bytes["OAGSFUKBCVWPJTEXYHDIQNLZMR "]...
};

int main()
{
  /*
    0. Supplied Message: "012222 1114142503 0313012513 03141418192"
                         "102 0113 2419182119021713 06131715070119"

    1. a) Transform: /\s/ -> "26"
       b) Transform: /(\d\d)/ -> "$1,"
       c) Transform: /0(\d)/ -> "$1"
  */

  std::cout << Decode<1,22,22,26,11,14,14,25,3,26,3,13,1,25,13,26,
                      3,14,14,18,19,21,2,26,1,13,26,24,19,18,21,19,
                      2,17,13,26,6,13,17,15,7,1,19>::message << '\n';
}
