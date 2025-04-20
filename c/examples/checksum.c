#include "gps.h"
#include <stdio.h>

// Example usage
int main() {
  // Sample NMEA messages
  const char *valid_msg   = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47";
  const char *invalid_msg = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*XX"; // Invalid checksum
  const char *no_checksum = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,";

  // Test valid message
  int result = validate_nmea_checksum(valid_msg);
  printf("Valid message: %s\n", result == 0 ? "Checksum OK" : "Checksum Error");
  if (result != 0) printf("Error code: %d\n", result);

  // Test invalid message
  result = validate_nmea_checksum(invalid_msg);
  printf("Invalid message: %s\n", result == 0 ? "Checksum OK" : "Checksum Error");
  if (result != 0) printf("Error code: %d\n", result);

  // Test message without checksum
  result = validate_nmea_checksum(no_checksum);
  printf("No checksum message: %s\n", result == 0 ? "Checksum OK" : "Checksum Error");
  if (result != 0) printf("Error code: %d\n", result);

  return 0;
}