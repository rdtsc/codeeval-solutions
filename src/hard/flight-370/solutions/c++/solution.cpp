#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <locale>
#include <map>
#include <vector>

struct Marker
{
  unsigned id;
  const char* name;
  unsigned weight;
  double x, y, timestamp;
};

struct Target
{
  Target() : radius(), x(), y() {}

  bool contains(const Marker& marker) const
  {
    const auto distance = [&]
    {
      static constexpr auto meanPlanetaryDiameter = 12742.0;

      // As per: https://en.wikipedia.org/wiki/Haversine_formula
      const auto lhs = ::sin((marker.x - this->x) / 2),
                 rhs = ::sin((marker.y - this->y) / 2);

      const auto phi    = (lhs * lhs),
                 lambda = (rhs * rhs);

      return meanPlanetaryDiameter * ::asin
      (
        ::sqrt(phi + ::cos(this->x) * ::cos(marker.x) * lambda)
      );
    }();

    return (distance <= this->radius);
  }

  friend std::istream& operator>>(std::istream& inputStream, Target& target)
  {
    static constexpr auto radians = (::acos(-1) / 180);

    inputStream >> target.radius >> target.y >> target.x;

    target.x *= radians;
    target.y *= radians;

    return inputStream;
  }

  unsigned radius;
  double x, y;
};

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  std::ifstream inputStream(argv[1]);

  assert(inputStream && "Failed to open input stream.");

  // Add semicolons, parentheses, and commas to the list of stream delimiters.
  inputStream.imbue(std::locale(inputStream.getloc(), []
  {
    using facet = std::ctype<char>;

    const auto classicTableStart = facet::classic_table(),
               classicTableEnd   = classicTableStart + facet::table_size;

    static std::vector<facet::mask> categories(classicTableStart,
                                               classicTableEnd);

    for(const auto c : ";(),") if(c) categories[c] |= facet::space;

    // Ref-counted upstream.
    return new facet(categories.data());
  }()));

  // The placemark database in the supplied input files is static. Saving some
  // time and pain by pre-parsing the XML. Note that coordinates are supplied
  // in the following order: (longitude, latitude)
  static const Marker markers[] =
  {
    /*
      #!/usr/bin/env python

      import sys
      import re
      import math
      import time

      from datetime import datetime
      from xml.dom import minidom

      def getRawXml(filename):
        with open(filename) as file:
          lines = file.read()
          return lines[lines.find('<?xml'):]

      def isValidPlacemark(marker):
        return len(marker.getElementsByTagName('name')) > 0

      def getPlacemarkId(marker):
        return marker.attributes['id'].value

      def getPlacemarkName(marker):
        return marker.getElementsByTagName('name')[0].firstChild.nodeValue

      def getPlacemarkCoordinates(marker):
        raw = marker.getElementsByTagName('coordinates')[0].firstChild.nodeValue
        return [math.radians(float(chunk)) for chunk in raw.split(',')]

      def getPlacemarkConfirmationCount(marker):
        raw = marker.getElementsByTagName('description')[0].firstChild.nodeValue
        selector = r'confirmation\s*:\s*<\s*b\s*>\s*(\d+)'
        match = re.search(selector, raw, re.IGNORECASE)
        return int(match.group(1)) if match else 0

      def getPlacemarkTimestamp(marker):
        raw = marker.getElementsByTagName('when')[0].firstChild.nodeValue
        date = datetime.strptime(raw, '%Y-%m-%d %H:%M:%S.%f')
        return time.mktime(date.timetuple()) + (date.microsecond / 1e6)

      xml = minidom.parseString(getRawXml(sys.argv[1]))
      placemarks = xml.getElementsByTagName('Placemark')

      markers = \
      (
        {
         'id'    : getPlacemarkId(marker),
         'name'  : getPlacemarkName(marker),
         'pos'   : getPlacemarkCoordinates(marker),
         'weight': getPlacemarkConfirmationCount(marker),
         'time'  : getPlacemarkTimestamp(marker)
        }

        for marker in filter(lambda m: isValidPlacemark(m), placemarks)
      )

      for marker in markers:
        print('{{{0}, "{1}", {2}, {3}, {4}, {5:.6f}}},'.format(marker['id'],
                                                               marker['name'],
                                                               marker['weight'],
                                                               marker['pos'][1],
                                                               marker['pos'][0],
                                                               marker['time']))
    */

    #include "markers.dat"
  };

  std::multimap<unsigned, const Marker*> sightings;
  std::vector<decltype(sightings)::mapped_type> results;

  for(Target target; inputStream >> target; std::cout << '\n')
  {
    // Find markers within the given search radius.
    {
      sightings.clear();

      for(const auto& marker : markers) if(target.contains(marker))
      {
        sightings.insert({marker.weight, &marker});
      }
    }

    if(sightings.empty())
    {
      std::cout << "None";
      continue;
    }

    // Select markers with the most sightings and sort them based on
    // timestamps and/or IDs.
    {
      results.clear();

      const auto maxSightings = sightings.crbegin()->second->weight;

      for(const auto& sighting : sightings)
      {
        if(sighting.first != maxSightings) continue;

        results.emplace_back(sighting.second);
      }

      using value_type = decltype(results)::value_type;

      std::sort(results.begin(), results.end(), [](const value_type lhs,
                                                   const value_type rhs)
      {
        static constexpr auto epsilon =
          std::numeric_limits<decltype(lhs->timestamp)>::epsilon();

        if(std::fabs(lhs->timestamp - rhs->timestamp) < epsilon)
        {
          return (lhs->id < rhs->id);
        }

        return (lhs->timestamp < rhs->timestamp);
      });
    }

    // Dump marker names.
    {
      const auto resultsBegin = results.cbegin(),
                 resultsEnd   = results.cend();

      std::cout << (*resultsBegin)->name;

      for(auto i = std::next(resultsBegin); i != resultsEnd; ++i)
      {
        std::cout << ", " << (*i)->name;
      }
    }
  }
}
