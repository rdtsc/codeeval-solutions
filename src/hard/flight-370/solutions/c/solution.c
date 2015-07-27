#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
  unsigned id;
  const char* name;
  unsigned weight;
  double x, y, timestamp;
} Marker;

typedef struct
{
  unsigned radius;
  double x, y;
} Target;

static bool targetUnserialize(Target* const target, FILE* const inputStream)
{
  assert(target && inputStream && !ferror(inputStream));

  #define ARGUMENT_PACK &target->radius, &target->y, &target->x

  if(fscanf(inputStream, "%u%*[^(](%lf,%lf)", ARGUMENT_PACK) != 3)
  {
    return false;
  }

  #undef ARGUMENT_PACK

  static const double radians = 0.01745329251994329576; // ~(Pi / 180)

  target->x *= radians;
  target->y *= radians;

  return true;
}

static bool targetContains(const Target* const target,
                           const Marker* const marker)
{
  assert(target && marker);

  static const double meanPlanetaryDiameter = 12742.0;

  // As per: https://en.wikipedia.org/wiki/Haversine_formula
  const double lhs = sin((marker->x - target->x) / 2),
               rhs = sin((marker->y - target->y) / 2);

  const double phi    = (lhs * lhs),
               lambda = (rhs * rhs);

  const double distance = meanPlanetaryDiameter * asin
  (
    sqrt(phi + cos(target->x) * cos(marker->x) * lambda)
  );

  return (distance <= target->radius);
}

static inline int markerOrdering(const void* const lhs, const void* const rhs)
{
  assert(lhs && rhs);

  const Marker* const x = *(const Marker* const *)lhs,
              * const y = *(const Marker* const *)rhs;

  #define COMPARE(Left, Right) ((Left) < (Right) ? (-1) : (Left) > (Right))

  if(fabs(x->timestamp - y->timestamp) < DBL_EPSILON)
  {
    return COMPARE(x->id, y->id);
  }

  return COMPARE(x->timestamp, y->timestamp);

  #undef COMPARE
}

int main(const int argc, const char* const argv[])
{
  // Getting away with no error checking throughout because CodeEval makes some
  // strong guarantees about our runtime environment. No need to pay when we're
  // being benchmarked. Don't forget to define NDEBUG prior to submitting!
  assert(argc >= 2 && "Expecting at least one command-line argument.");

  static char stdoutBuffer[512] = "";

  // Turn on full output buffering for stdout.
  setvbuf(stdout, stdoutBuffer, _IOFBF, sizeof stdoutBuffer);

  FILE* inputStream = fopen(argv[1], "r");

  assert(inputStream && "Failed to open input stream.");

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

  static const size_t markerCount = (sizeof markers / sizeof *markers);

  for(Target target = {0}; targetUnserialize(&target, inputStream);)
  {
    const Marker* sightings[2048] = {0};
    size_t sightingCount = 0;
    unsigned maxSightings = 0;

    // Find markers within the given search radius.
    for(size_t i = 0; i < markerCount; ++i)
    {
      const Marker* const marker = &markers[i];

      if(targetContains(&target, marker))
      {
        assert(sightingCount < (sizeof sightings / sizeof *sightings));

        sightings[sightingCount++] = marker;

        if(marker->weight > maxSightings)
        {
          maxSightings = marker->weight;
        }
      }
    }

    if(!sightingCount)
    {
      puts("None");
      continue;
    }

    const Marker* results[sizeof sightings / sizeof *sightings] = {0};
    size_t resultCount = 0;

    // Select markers with the most sightings and sort them based on
    // timestamps and/or IDs.
    {
      for(size_t i = 0; i < sightingCount; ++i)
      {
        const Marker* const sighting = sightings[i];

        if(sighting->weight != maxSightings) continue;

        assert(resultCount < (sizeof results / sizeof *results));

        results[resultCount++] = sighting;
      }

      qsort(results, resultCount, sizeof *results, markerOrdering);
    }

    // Dump marker names.
    {
      fputs((*results)->name, stdout);

      for(size_t i = 1; i < resultCount; ++i)
      {
        fputs(", ", stdout);
        fputs(results[i]->name, stdout);
      }

      putchar('\n');
    }
  }

  // The CRT takes care of cleanup.
}
