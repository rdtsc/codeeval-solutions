#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>

static void registerStreamDelimiters(std::istream& inputStream,
                                     const std::string& delimiters)
{
  assert(!delimiters.empty());

  inputStream.imbue(std::locale(inputStream.getloc(), [&]
  {
    using facet = std::ctype<char>;

    auto* const categories = new facet::mask[facet::table_size]();

    std::copy_n(facet::classic_table(), facet::table_size, categories);

    for(const std::size_t i : delimiters) categories[i] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories);
  }()));
}

class CodeEvalFacet : public std::numpunct<char>
{
  protected: virtual std::string do_truename() const override
  {
    return "True";
  }

  protected: virtual std::string do_falsename() const override
  {
    return "False";
  }
};

class Uri
{
  public: Uri() : port()
  {
    // Add colons and forward slashes to the list of stream delimiters.
    ::registerStreamDelimiters(this->tokenizer, ":/");
  }

  private: friend std::istream& operator>>(std::istream& inputStream, Uri& uri)
  {
    std::string rawUri;

    if(inputStream >> rawUri) uri.unpack(rawUri);

    return inputStream;
  }

  private: void unpack(const std::string& rawUri)
  {
    assert(!rawUri.empty());

    this->resetTokenizer(rawUri);

    // Can't extract <scheme>://<host>:<port>
    if(!(this->tokenizer >> this->scheme >> this->host >> this->port))
    {
      this->resetTokenizer();

      // Can't extract <scheme>://<host>
      if(!(this->tokenizer >> this->scheme >> this->host)) assert(false);

      // Fetch the remaining path.
      std::getline(this->tokenizer, this->path);

      // As per the problem statement.
      this->port = 80;
    }

    // Fetch the remaining path.
    else std::getline(this->tokenizer, this->path);

    if(this->path.empty()) this->path = "/";

    assert(this->port < 65536U);

    this->normalize();
  }

  private: void normalize()
  {
    static const auto toLower = [](std::string& string)
    {
      std::transform(string.begin(), string.end(), string.begin(), ::tolower);
    };

    toLower(this->scheme);
    toLower(this->host);

    std::string decodedPath;

    const auto pathEnd = this->path.cend();

    for(auto i = this->path.cbegin(); i != pathEnd; ++i)
    {
      // URL-encoded character.
      if(*i == '%' && ::isxdigit(*(i + 1)))
      {
        const unsigned octet = [&]
        {
          unsigned char lhs = *(++i);
          unsigned char rhs = *(++i);

          #define HEX_DIGIT(Char) do {                                   \
            if(::isdigit(Char)) (Char) -= '0';                           \
            else (Char) = ((Char) - (::isupper(Char) ? 'A' : 'a')) + 10; \
          } while(false)

          HEX_DIGIT(lhs);
          HEX_DIGIT(rhs);

          #undef HEX_DIGIT

          return ((lhs << 4) + rhs);
        }();

        assert(::isprint(octet));

        decodedPath += static_cast<char>(octet);
      }

      else decodedPath += *i;
    }

    decodedPath.swap(this->path);
  }

  private: void resetTokenizer()
  {
    this->tokenizer.clear();
    this->tokenizer.seekg(0, std::ios::beg);
  }

  private: void resetTokenizer(const std::string& contents)
  {
    this->tokenizer.clear();
    this->tokenizer.str(contents);
  }

  public: bool operator==(const Uri& other) const
  {
    #define IS_SAME(Component) (this->Component == other.Component)

    return IS_SAME(port)   &&
           IS_SAME(scheme) &&
           IS_SAME(host)   &&
           IS_SAME(path);

    #undef IS_SAME
  }

  private: unsigned port;
  private: std::string scheme;
  private: std::string host;
  private: std::string path;
  private: std::istringstream tokenizer;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Add semicolons to the list of stream delimiters.
  ::registerStreamDelimiters(inputStream, ";");

  // Set bools to be emitted as their textual equivalents in English.
  std::cout.imbue(std::locale(std::cout.getloc(), [&]
  {
    std::boolalpha(std::cout);

    return new CodeEvalFacet();
  }()));

  for(Uri lhs, rhs; inputStream >> lhs >> rhs;)
  {
    std::cout << (lhs == rhs) << '\n';
  }
}
