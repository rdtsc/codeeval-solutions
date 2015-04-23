#include <array>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

using Ipv4Address = std::array<unsigned char, 4>;

std::istream& operator>>(std::istream& inputStream, Ipv4Address& address)
{
  static constexpr auto ignoreLimit =
    std::numeric_limits<std::streamsize>::max();

  const auto addressWidth = address.size();

  for(std::size_t i = 0; i < addressWidth; ++i)
  {
    unsigned byte = 0;

    inputStream >> byte;

    if(i < 3) inputStream.ignore(ignoreLimit, '.');

    assert(byte < 256);

    address[i] = static_cast<Ipv4Address::value_type>(byte);
  }

  return inputStream;
}

class Ipv4Header
{
  // IPv4 headers are 20 bytes wide. The layout should be available here:
  // http://en.wikipedia.org/wiki/IPv4#Header

  public: using value_type = unsigned char;

  public: void setRoute(const Ipv4Address& src, const Ipv4Address& dst)
  {
    // Inject the new source and destination addresses.
    for(std::size_t i = 0; i < 4; ++i)
    {
      this->at(12 + i) = src[i];
      this->at(16 + i) = dst[i];
    }

    this->updateChecksum();
  }

  private: void updateChecksum()
  {
    // Discard the provided header checksum.
    this->at(10) = this->at(11) = 0;

    unsigned sum = 0;

    // Construct the header checksum as per:
    // http://en.wikipedia.org/wiki/IPv4_header_checksum
    for(std::size_t i = 0; i < Ipv4Header::width; i += 2)
    {
      sum += (this->at(i) << 8) + this->at(i + 1);
    }

    while(sum >> 16) sum = (sum & 0xFFFF) + (sum >> 16);

    sum = ~sum;

    this->at(10) = (sum & 0xFF00) >> 8;
    this->at(11) = (sum & 0x00FF) >> 0;
  }

  private: value_type& operator[](const std::size_t index)
  {
    return this->at(index);
  }

  private: value_type operator[](const std::size_t index) const
  {
    return this->at(index);
  }

  private: value_type& at(const std::size_t index)
  {
    assert(index < Ipv4Header::width);

    return this->data[index];
  }

  private: value_type at(const std::size_t index) const
  {
    assert(index < Ipv4Header::width);

    return this->data[index];
  }

  private: friend std::istream&
    operator>>(std::istream& inputStream, Ipv4Header& header)
  {
    static constexpr auto ipRouteWidth = 8;

    const auto headerWidth = Ipv4Header::width;

    assert(headerWidth > ipRouteWidth);

    const auto origStreamFlags = inputStream.flags();

    std::hex(inputStream);

    for(std::size_t i = 0; i < headerWidth - ipRouteWidth; ++i)
    {
      unsigned byte = 0;

      inputStream >> byte;

      assert(byte < 256);

      header[i] = static_cast<Ipv4Header::value_type>(byte);
    }

    inputStream.flags(origStreamFlags);

    return inputStream;
  }

  private: friend std::ostream&
    operator<<(std::ostream& outputStream, const Ipv4Header& header)
  {
    const auto origStreamFlags = outputStream.flags();
    const auto origStreamFillChar = outputStream.fill();

    std::hex(outputStream);
    outputStream.fill('0');

    outputStream << std::setw(2) << static_cast<unsigned>(header[0]);

    for(std::size_t i = 1; i < Ipv4Header::width; ++i)
    {
      outputStream << ' ' << std::setw(2) << static_cast<unsigned>(header[i]);
    }

    outputStream.flags(origStreamFlags);
    outputStream.fill(origStreamFillChar);

    return outputStream;
  }

  public: static constexpr std::size_t width = 20;
  private: std::array<value_type, width> data;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  std::istringstream tokenizer;
  char payloadBuffer[8192] = "";

  while(true)
  {
    inputStream.getline(payloadBuffer, sizeof payloadBuffer - 1);

    if(!inputStream) break;

    Ipv4Address source, destination;
    Ipv4Header header;

    // The packet header fits well inside of this threshold. See:
    // http://en.wikipedia.org/wiki/IPv4#Header
    payloadBuffer[128] = '\0';

    tokenizer.clear();
    tokenizer.str(payloadBuffer);
    tokenizer >> source >> destination >> header;

    header.setRoute(source, destination);

    std::cout << header << '\n';
  }
}
