#include <cassert>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>

struct Rgb
{
  Rgb() : r(), g(), b() {}

  void fromHex(const std::string& hex)
  {
    assert((hex.size() >= 7) && (hex.front() == '#'));

    const auto digit = [&](const std::size_t index)
    {
      const char c = hex[index];

      // Guaranteed to contain lowercase xdigits by the problem statement.
      assert(::isxdigit(c));

      if(::isdigit(c)) return (c - '0');

      return (::tolower(c) - 'a' + 10);
    };

    this->r = (digit(1) << 4) + digit(2);
    this->g = (digit(3) << 4) + digit(4);
    this->b = (digit(5) << 4) + digit(6);
  }

  void fromCmyk(const std::string& cmyk)
  {
    assert((cmyk.size() >= 9) && (cmyk.front() == '('));

    std::istringstream tokenizer(cmyk);

    double c = 0, m = 0, y = 0, k = 0;

    const auto status = [&]() -> bool
    {
      char eat = '\0';

      return (tokenizer >> eat >> c >> eat >> m >> eat >> y >> eat >> k);
    }();

    if(!status) assert(false);

    // NOTE: This conversion is senseless without a proper color profile as
    // the boundaries of the RGB and CMYK gamuts are variable. Below is a
    // basic approximation.

    const double black = ((1 - k) * 255);

    this->r = ::round((1 - c) * black);
    this->g = ::round((1 - m) * black);
    this->b = ::round((1 - y) * black);
  }

  void fromHsv(const std::string& hsv)
  {
    assert((hsv.size() >= 10) && !hsv.compare(0, 3, "HSV"));

    std::istringstream tokenizer(hsv.substr(3));

    double h = 0, s = 0, v = 0;

    const auto status = [&]() -> bool
    {
      char eat = '\0';

      return (tokenizer >> eat >> h >> eat >> s >> eat >> v);
    }();

    if(!status) assert(false);

    // As described in: https://en.wikipedia.org/wiki/HSL_and_HSV#From_HSV

    h /= 6e1;
    s /= 1e2;
    v /= 1e2;

    const auto c = (v * s),
               m = (v - c),
               x = (c * (1 - ::fabs(::fmod(h, 2) - 1)));

    this->fromHsx(h, c, m, x);
  }

  void fromHsl(const std::string& hsl)
  {
    assert((hsl.size() >= 10) && !hsl.compare(0, 3, "HSL"));

    std::istringstream tokenizer(hsl.substr(3));

    double h = 0, s = 0, l = 0;

    const auto status = [&]() -> bool
    {
      char eat = '\0';

      return (tokenizer >> eat >> h >> eat >> s >> eat >> l);
    }();

    if(!status) assert(false);

    // As described in: https://en.wikipedia.org/wiki/HSL_and_HSV#From_HSL

    h /= 6e1;
    s /= 1e2;
    l /= 1e2;

    const auto c = ((1 - ::fabs(2 * l - 1)) * s),
               m = (l - c / 2),
               x = (c * (1 - ::fabs(::fmod(h, 2) - 1)));

    this->fromHsx(h, c, m, x);
  }

  void fromHsx(const double h, const double c, const double m, const double x)
  {
    // As described in: https://en.wikipedia.org/wiki/HSL_and_HSV#From_HSV

    const auto hueInRange = [&](const double lo, const double hi)
    {
      return (h >= lo && h < hi);
    };

    const auto select = [&](const double r, const double g, const double b)
    {
      this->r = ::round((r + m) * 255);
      this->g = ::round((g + m) * 255);
      this->b = ::round((b + m) * 255);
    };

    if     (hueInRange(0, 1)) select(c, x, 0);
    else if(hueInRange(1, 2)) select(x, c, 0);
    else if(hueInRange(2, 3)) select(0, c, x);
    else if(hueInRange(3, 4)) select(0, x, c);
    else if(hueInRange(4, 5)) select(x, 0, c);
    else if(hueInRange(5, 6)) select(c, 0, x);
    else                      select(0, 0, 0);
  }

  friend std::ostream& operator<<(std::ostream& outputStream, const Rgb& rgb)
  {
    const unsigned r = rgb.r, g = rgb.g, b = rgb.b;

    return outputStream << "RGB(" << r << ',' << g << ',' << b << ')';
  }

  unsigned char r, g, b;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  for(std::string line; std::getline(inputStream, line);)
  {
    // Sanity check.
    assert(line.size() >= 7);

    Rgb rgb;

    switch(line.front())
    {
      case '#': rgb.fromHex(line);  break;
      case '(': rgb.fromCmyk(line); break;

      case 'H': switch(line[2])
      {
        case 'V': rgb.fromHsv(line); break;
        case 'L': rgb.fromHsl(line); break;
      }
      break;
    }

    std::cout << rgb << '\n';
  }
}
