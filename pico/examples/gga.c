#include "gps.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Example usage
int main() {
  const char *sample_gga = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47";
  gga_t data             = {0}; // Initialize struct

  int result = parse_gga(sample_gga, &data);
  if (result == 0) {
    printf("Time: %s\n", data.time);
    printf("Latitude: %.6f %c\n", data.latitude, data.lat_dir);
    printf("Longitude: %.6f %c\n", data.longitude, data.lon_dir);
    printf("Fix Quality: %d\n", data.fix_quality);
    printf("Satellites: %d\n", data.satellites);
    printf("HDOP: %.1f\n", data.hdop);
    printf("Altitude: %.1f %c\n", data.altitude, data.alt_unit);
    printf("Geoid Separation: %.1f %c\n", data.geoid_separation, data.geoid_unit);
  }
  else {
    printf("Error parsing GGA message: %d\n", result);
  }

  return 0;
}