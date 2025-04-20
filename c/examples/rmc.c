#include "gps.h"
#include <stdio.h>

// Example usage
int main() {
  // const char *sample_rmc = "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A";
  const char *sample_rmc = "$GNRMC,001031.00,A,4404.13993,N,12118.86023,W,0.146,,100117,,,A*7B";
  rmc_t data             = {0}; // Initialize struct

  int result = parse_rmc(sample_rmc, &data);
  if (result == 0) {
    printf("Time: %s\n", data.time);
    printf("Status: %c\n", data.status);
    printf("Latitude: %.6f %c\n", data.latitude, data.lat_dir);
    printf("Longitude: %.6f %c\n", data.longitude, data.lon_dir);
    printf("Speed: %.1f knots\n", data.speed);
    printf("Course: %.1f degrees\n", data.course);
    printf("Date: %s\n", data.date);
    printf("Magnetic Variation: %.1f %c\n", data.mag_var, data.mag_var_dir);
  }
  else {
    printf("Error parsing RMC message: %d\n", result);
  }

  return 0;
}