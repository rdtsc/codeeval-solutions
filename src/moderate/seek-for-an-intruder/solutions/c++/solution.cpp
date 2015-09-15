#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

// Non-STDC.
#include <regex.h>

// NOTE: Exact-width types are not guaranteed to be available on all platforms.
using ipv4_t = std::uint32_t;

struct Ipv4Filter
{
  Ipv4Filter(const char* const pattern,
             bool(* const check)(const char*),
             ipv4_t(* const compact)(const char*)) : pattern(pattern),
                                                     isValidIpv4(check),
                                                     getCompact(compact)
  {
    static const auto regexFlags = (REG_EXTENDED | REG_ICASE);

    if(::regcomp(&this->spec, pattern, regexFlags))
    {
      throw std::runtime_error("Failed to compile pattern.");
    }
  }

  const char* const pattern;
  regex_t spec;
  bool(* const isValidIpv4)(const char*);
  ipv4_t(* const getCompact)(const char*);
};

template<unsigned Base> struct Ipv4Util
{
  static ipv4_t getCompactedBase(const char* const address)
  {
    assert(address && *address);

    return ::strtoul(address, NULL, Base);
  }

  static ipv4_t getCompactedDotted(const char* address)
  {
    assert(address && *address);

    ipv4_t result = 0;

    char* cursor = NULL;

    for(unsigned char shift = 4; shift--; address = ++cursor)
    {
      const unsigned octet = ::strtoul(address, &cursor, Base);

      if((octet >= (1 << 8)) || (shift && (*cursor != '.'))) return 0;

      result += ((1 << (8 * shift)) * octet);
    }

    return result;
  }

  static bool isValidBase(const char* const address)
  {
    assert(address && *address);

    const ipv4_t ip = getCompactedBase(address);

    return (ip >= (1U << 24) && ip <= ((1U << 31) * 2 - 2));
  }

  static bool isValidDotted(const char* const address)
  {
    assert(address && *address);

    const ipv4_t ip = getCompactedDotted(address);

    return (ip >= (1U << 24) && ip <= ((1U << 31) * 2 - 2));
  }
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  static const Ipv4Filter filters[] =
  {
    #define FILTER_BASE(Base, Regex) { \
      "(" Regex ")",                   \
      Ipv4Util<Base>::isValidBase,     \
      Ipv4Util<Base>::getCompactedBase \
    }

    #define FILTER_DOTTED(Base, Regex) {                 \
      "(" Regex "\\." Regex "\\." Regex "\\." Regex ")", \
      Ipv4Util<Base>::isValidDotted,                     \
      Ipv4Util<Base>::getCompactedDotted                 \
    }

    FILTER_BASE(2,  "[01]+"),
    FILTER_BASE(8,  "0[0-7]+"),
    FILTER_BASE(10, "[0-9]+"),
    FILTER_BASE(16, "0x[0-9a-f]+"),

    FILTER_DOTTED(2,  "[01]+"),
    FILTER_DOTTED(8,  "0[0-9]+"),
    FILTER_DOTTED(10, "[0-9]+"),
    FILTER_DOTTED(16, "0x[0-9a-f]+"),

    #undef FILTER_DOTTED
    #undef FILTER_BASE
  };

  std::unordered_map<ipv4_t, std::size_t> hits;
  std::istringstream tokenizer;

  for(std::string line; std::getline(inputStream, line);)
  {
    // Filter garbage.
    std::replace_if(line.begin(), line.end(), [](const char c)
    {
      return (!::isxdigit(c) && (c != '.') && (c != 'x'));
    }, ' ');

    // Isolate plausible candidates.
    for(tokenizer.clear(), tokenizer.str(line); tokenizer >> line;)
    {
      if(line.size() < (sizeof "0.0.0.0" - 1)) continue;

      // Detect and record IPs.
      for(const auto& filter : filters)
      {
        regmatch_t group = {0, 0};

        // Try different filters until we find a match.
        const auto isDone = [&]
        {
          for(const char* cursor = line.c_str();
              !regexec(&filter.spec, cursor, 1, &group, 0);
              ++cursor)
          {
            static char match[64] = "";

            ::memcpy(match, &cursor[group.rm_so], (group.rm_eo - group.rm_so));

            match[group.rm_eo - group.rm_so] = '\0';

            if(filter.isValidIpv4(match))
            {
              ++hits[filter.getCompact(match)];

              return true;
            }
          }

          return false;
        }();

        if(isDone) break;
      }
    }
  }

  assert(!hits.empty());

  // Find the first most frequently occurring IP(s).
  const auto results = [&]
  {
    using value_type = decltype(hits)::value_type;

    // Find the highest IP occurrence rate.
    const auto hitRate = std::max_element(hits.cbegin(), hits.cend(),
      [&](const value_type& lhs, const value_type& rhs)
    {
      return (lhs.second < rhs.second);
    })->second;

    std::vector<ipv4_t> results;

    for(const auto& hit : hits) if(hit.second == hitRate)
    {
      results.emplace_back(hit.first);
    }

    if(results.size() > 1) std::sort(results.begin(), results.end());

    return results;
  }();

  // Dump results.
  for(const auto ip : results)
  {
    const auto octet = [&](const std::size_t offset)
    {
      return ((ip >> (24 - (offset << 3))) & 0xFF);
    };

    std::cout << octet(0) << '.' << octet(1) << '.'
              << octet(2) << '.' << octet(3) << ' ';
  }

  std::cout << '\n';
}
