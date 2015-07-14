#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
  unsigned char r, g, b;
} Rgb, Hex;

typedef struct
{
  double c, m, y, k;
} Cmyk;

typedef struct
{
  unsigned h;
  unsigned char s, v;
} Hsv;

typedef struct
{
  unsigned h;
  unsigned char s, l;
} Hsl;

static Rgb unserializeHex(const char* const data)
{
  assert(data && (*data == '#'));

  Rgb result = {0};

  #define COMPONENTS &result.r, &result.g, &result.b

  if(sscanf(data, "#%2hhx%2hhx%2hhx", COMPONENTS) != 3) assert(false);

  #undef COMPONENTS

  return result;
}

static Rgb HexToRgb(const Hex* const hex)
{
  return (*hex);
}

static Cmyk unserializeCmyk(const char* const data)
{
  assert(data && (*data == '('));

  Cmyk result = {.c = 0};

  #define COMPONENTS &result.c, &result.m, &result.y, &result.k

  if(sscanf(data, "(%lf,%lf,%lf,%lf)", COMPONENTS) != 4) assert(false);

  #undef COMPONENTS

  return result;
}

static Rgb CmykToRgb(const Cmyk* const cmyk)
{
  assert(cmyk);

  // NOTE: This conversion is senseless without a proper color profile as
  // the boundaries of the RGB and CMYK gamuts are variable. Below is a
  // basic approximation.

  const double black = ((1 - cmyk->k) * 255);

  const Rgb result =
  {
    .r = round((1 - cmyk->c) * black),
    .g = round((1 - cmyk->m) * black),
    .b = round((1 - cmyk->y) * black)
  };

  return result;
}

static Hsv unserializeHsv(const char* const data)
{
  assert(data && (*data == 'H'));

  Hsv result = {.h = 0};

  #define COMPONENTS &result.h, &result.s, &result.v

  if(sscanf(data, "HSV(%u,%hhu,%hhu)", COMPONENTS) != 3) assert(false);

  #undef COMPONENTS

  return result;
}

static Rgb HsvToRgb(const Hsv* const hsv)
{
  assert(hsv);

  // As described in: https://en.wikipedia.org/wiki/HSL_and_HSV#From_HSV

  const double h = (hsv->h / 6e1),
               s = (hsv->s / 1e2),
               v = (hsv->v / 1e2);

  const double c = (v * s),
               m = (v - c),
               x = (c * (1 - fabs(fmod(h, 2) - 1)));

  Rgb result = {0};

  #define HUE_IN_RANGE(Lo, Hi) (h >= (Lo) && h < (Hi))

  #define SELECT(R, G, B) do {         \
    result.r = round(((R) + m) * 255); \
    result.g = round(((G) + m) * 255); \
    result.b = round(((B) + m) * 255); \
  } while(false)

  if     (HUE_IN_RANGE(0, 1)) SELECT(c, x, 0);
  else if(HUE_IN_RANGE(1, 2)) SELECT(x, c, 0);
  else if(HUE_IN_RANGE(2, 3)) SELECT(0, c, x);
  else if(HUE_IN_RANGE(3, 4)) SELECT(0, x, c);
  else if(HUE_IN_RANGE(4, 5)) SELECT(x, 0, c);
  else if(HUE_IN_RANGE(5, 6)) SELECT(c, 0, x);

  #undef SELECT
  #undef HUE_IN_RANGE

  return result;
}

static Hsl unserializeHsl(const char* const data)
{
  assert(data && (*data == 'H'));

  Hsl result = {.h = 0};

  #define COMPONENTS &result.h, &result.s, &result.l

  if(sscanf(data, "HSL(%u,%hhu,%hhu)", COMPONENTS) != 3) assert(false);

  #undef COMPONENTS

  return result;
}

static Rgb HslToRgb(const Hsl* const hsl)
{
  assert(hsl);

  // As described in: https://en.wikipedia.org/wiki/HSL_and_HSV#From_HSL

  const double h = (hsl->h / 6e1),
               s = (hsl->s / 1e2),
               l = (hsl->l / 1e2);

  const double c = ((1 - fabs(2 * l - 1)) * s),
               m = (l - c / 2),
               x = (c * (1 - fabs(fmod(h, 2) - 1)));

  Rgb result = {0};

  #define HUE_IN_RANGE(Lo, Hi) (h >= (Lo) && h < (Hi))

  #define SELECT(R, G, B) do {         \
    result.r = round(((R) + m) * 255); \
    result.g = round(((G) + m) * 255); \
    result.b = round(((B) + m) * 255); \
  } while(false)

  if     (HUE_IN_RANGE(0, 1)) SELECT(c, x, 0);
  else if(HUE_IN_RANGE(1, 2)) SELECT(x, c, 0);
  else if(HUE_IN_RANGE(2, 3)) SELECT(0, c, x);
  else if(HUE_IN_RANGE(3, 4)) SELECT(0, x, c);
  else if(HUE_IN_RANGE(4, 5)) SELECT(x, 0, c);
  else if(HUE_IN_RANGE(5, 6)) SELECT(c, 0, x);

  #undef SELECT
  #undef HUE_IN_RANGE

  return result;
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[1024] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

  for(char lineBuffer[32] = "";
      fgets(lineBuffer, sizeof lineBuffer, inputStream);)
  {
    // Sanity check.
    assert(strlen(lineBuffer) >= 7);

    Rgb rgb = {0};

    switch(*lineBuffer)
    {
      #define CONVERT(Type) do {                               \
        const Type type##Data = unserialize##Type(lineBuffer); \
        rgb = Type##ToRgb(&type##Data);                        \
      } while(false)

      case '#': CONVERT(Hex);  break;
      case '(': CONVERT(Cmyk); break;

      case 'H': switch(lineBuffer[2])
      {
        case 'V': CONVERT(Hsv); break;
        case 'L': CONVERT(Hsl); break;
      }
      break;

      #undef CONVERT
    }

    printf("RGB(%u,%u,%u)\n", rgb.r, rgb.g, rgb.b);
  }

  // The CRT takes care of cleanup.
}
