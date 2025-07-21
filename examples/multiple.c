#include "gci_gps.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum {
  GPS_UNKNOWN, // 0
  GPS_GGA,     // 1
  GPS_GSA,     // 2
  GPS_RMC,     // 3
  UBLOX        // 4
} gps_msg_type_t;

// Function to determine message type and process it
gps_msg_type_t process_gps_message(const void *data, size_t len) {
  if (data == NULL) return GPS_UNKNOWN;

  const char *msg = (const char *)data;
  printf("good data: %s\n", msg);
  if ((len > 0) && (msg[0] == '$') && (msg[1] == 'G')) {
    printf("Detected NEMA message\n");
    if (strncmp(&msg[3], "RMC", 3) == 0) return GPS_RMC;
    if (strncmp(&msg[3], "GSA", 3) == 0) return GPS_GSA;
    if (strncmp(&msg[3], "GGA", 3) == 0) return GPS_GGA;
  }
  const uint8_t *bytes = (const uint8_t *)data;
  if (len >= 2 && bytes[0] == 0xB5 && bytes[1] == 0x62) {
      // Likely UBX
      printf("Detected UBX message:\n");
      return UBLOX;
  }
  return GPS_UNKNOWN;
}


// Example usage
int main() {
  const char *sample_rmc = "$GNRMC,001031.00,A,4404.13993,N,12118.86023,W,0.146,,100117,,,A*7B";
  const char *sample_gsa = "$GPGSA,A,3,19,28,14,18,27,22,31,39,,,,,1.7,1.0,1.3*35";
  const char *sample_gga = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47";
  const uint8_t sample_ublox[] = {0xB5, 0x62,0x01, 0x07,0x5C,0x00,0x00,0x00,0x00,0x00,0xE5,0x07,0x06,0x1C,0x16, 0x2B, 0x00,0x07,0x12, 0x34};
  
  const char *msgs[4] = {
    sample_rmc,
    sample_gsa,
    sample_gga,
    (const char*)sample_ublox
  };

  for (int i=0; i<4; ++i) {
    const char* msg = msgs[i];
    gps_msg_type_t type = process_gps_message(msg, strlen(msg));

    if (type == GPS_RMC) {
      printf("Found RMC message --------------------\n");
      rmc_t rmc;
      int result = parse_rmc(sample_rmc, &rmc);
      print_rmc(rmc);
    }
    else if (type == GPS_GGA) {
      printf("Found GGA message -------------------\n");
      gga_t gga;
      int result = parse_gga(sample_gga, &gga);
      print_gga(gga);
    }
    else if (type == GPS_GSA) {
      printf("Found GSA message ------------------\n");
      gsa_t gsa;
      int result = parse_gsa(sample_gsa, &gsa);
      print_gsa(gsa);
    }
    else if (type == UBLOX) {
      printf("Found UBLOX\n");
    }
    else {
      printf("Error, message type: %d\n", type);
    }
  }

  // int result = parse_gga(sample_gga, &data);
  // if (result == 0) {
  //   printf("Time: %s\n", data.time);
  //   printf("Latitude: %.6f %c\n", data.latitude, data.lat_dir);
  //   printf("Longitude: %.6f %c\n", data.longitude, data.lon_dir);
  //   printf("Fix Quality: %d\n", data.fix_quality);
  //   printf("Satellites: %d\n", data.satellites);
  //   printf("HDOP: %.1f\n", data.hdop);
  //   printf("Altitude: %.1f %c\n", data.altitude, data.alt_unit);
  //   printf("Geoid Separation: %.1f %c\n", data.geoid_separation, data.geoid_unit);
  // }
  // else {
  //   printf("Error parsing GGA message: %d\n", result);
  // }

  return 0;
}