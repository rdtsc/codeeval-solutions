#include <algorithm>
#include <bitset>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

template<std::size_t Capacity> class KeyIterator
{
  public: KeyIterator() : key("0"), width(1), bits()
  {
    static_assert((Capacity > 3), "Bitset capacity should be reasonable.");
  }

  public: std::string operator*() const
  {
    return this->key;
  }

  public: KeyIterator& operator++()
  {
    this->bits = (this->bits.to_ullong() + 1ULL);

    // All bits are set; inflate and start counting from zero.
    if(this->bits.count() == this->width)
    {
      ++this->width;
      this->bits.reset();
    }

    this->key = this->bits.to_string();

    // Trim leading zeros.
    this->key = this->key.substr(this->key.size() - this->width);

    return *this;
  }

  public: KeyIterator operator++(int)
  {
    auto clone(*this);

    ++(*this);

    return clone;
  }

  private: std::string key;
  private: std::size_t width;
  private: std::bitset<Capacity> bits;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::unordered_map<std::string, char> mapping;
  std::string header, data, needle;

  const auto isNeedleTerminator = [&](const std::size_t width)
  {
    const auto setBitCount = static_cast<std::size_t>
      (std::count(needle.cbegin(), needle.cend(), '1'));

    return (setBitCount == width);
  };

  for(std::string line; std::getline(inputStream, line); std::cout << '\n')
  {
    // Encoding terminator, as per the problem statement.
    assert(line.substr(line.size() - 3) == "000");

    // Explode chunks.
    {
      const auto cursor = line.find_first_of("01");

      assert(cursor != std::string::npos);

      header = line.substr(0, cursor);
      data   = line.substr(cursor);
    }

    // Generate mapping.
    {
      KeyIterator<8> key;

      mapping.clear();

      for(const char c : header) mapping[*key++] = c;
    }

    // Decode message.
    while(!data.empty())
    {
      // Width is in base 2.
      const auto width = std::stoul(data.substr(0, 3), nullptr, 2);

      // Segment header.
      data = data.substr(3);

      for(;;)
      {
        needle = data.substr(0, width);
        data   = data.substr(width);

        if(isNeedleTerminator(width)) break;

        std::cout << mapping[needle];
      }
    }
  }
}
