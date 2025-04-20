////////////////////////////////////////////////
//  The MIT License (MIT)
//  Copyright (c) 2023 Kevin Walchko
//  see LICENSE for full details
////////////////////////////////////////////////
#include "gps.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Function to validate the checksum of an NMEA message
// Returns 0 if valid, non-zero if invalid or error
int validate_nmea_checksum(const char *nmea_msg) {
  // Check for valid input
  if (!nmea_msg || nmea_msg[0] != '$') {
    return -1; // Invalid input or missing start character
  }

  // Find the checksum delimiter (*)
  const char *checksum_start = strchr(nmea_msg, '*');
  if (!checksum_start || checksum_start == nmea_msg) {
    return -2; // Missing or invalid checksum delimiter
  }

  // Extract the provided checksum (2 hex digits after *)
  char provided_checksum_str[3] = {0};
  if (strlen(checksum_start) < 3) {
    return -3; // Checksum field too short
  }
  strncpy(provided_checksum_str, checksum_start + 1, 2);
  provided_checksum_str[2] = '\0';

  // Validate checksum characters (must be hex digits)
  if (!isxdigit((unsigned char)provided_checksum_str[0]) ||
      !isxdigit((unsigned char)provided_checksum_str[1])) {
    return -4; // Invalid checksum characters
  }

  // Convert provided checksum to integer
  unsigned char provided_checksum = (unsigned char)strtol(provided_checksum_str, NULL, 16);

  // Calculate the checksum (XOR of all characters between $ and *)
  unsigned char calculated_checksum = 0;
  const char *ptr                   = nmea_msg + 1; // Start after $
  while (ptr < checksum_start && *ptr != '\0') {
    calculated_checksum ^= (unsigned char)*ptr;
    ptr++;
  }

  // Compare calculated and provided checksums
  if (calculated_checksum != provided_checksum) {
    return -5; // Checksum mismatch
  }

  return 0; // Checksum valid
}

// // Example usage
// int main() {
//     // Sample NMEA messages
//     const char *valid_msg = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47";
//     const char *invalid_msg = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*XX"; // Invalid checksum
//     const char *no_checksum = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,";

//     // Test valid message
//     int result = validate_nmea_checksum(valid_msg);
//     printf("Valid message: %s\n", result == 0 ? "Checksum OK" : "Checksum Error");
//     if (result != 0) printf("Error code: %d\n", result);

//     // Test invalid message
//     result = validate_nmea_checksum(invalid_msg);
//     printf("Invalid message: %s\n", result == 0 ? "Checksum OK" : "Checksum Error");
//     if (result != 0) printf("Error code: %d\n", result);

//     // Test message without checksum
//     result = validate_nmea_checksum(no_checksum);
//     printf("No checksum message: %s\n", result == 0 ? "Checksum OK" : "Checksum Error");
//     if (result != 0) printf("Error code: %d\n", result);

//     return 0;
// }