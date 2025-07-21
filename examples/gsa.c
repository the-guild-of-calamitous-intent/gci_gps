#include "gci_gps.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Example usage
int main() {
  const char *sample_gsa = "$GPGSA,A,3,19,28,14,18,27,22,31,39,,,,,1.7,1.0,1.3*35";
  // const char *sample_gsa = "$GNGSA,A,3,80,71,73,79,69,,,,,,,,1.83,1.09,1.47*17";
  gsa_t data = {0}; // Initialize struct

  int result = parse_gsa(sample_gsa, &data);
  if (result == 0) {
    printf("Mode: %c\n", data.mode);
    printf("Fix Type: %d\n", data.fix_type);
    printf("Satellites Used: ");
    for (int i = 0; i < 12 && data.satellites[i] != 0; i++) {
      printf("%d ", data.satellites[i]);
    }
    printf("\n");
    printf("PDOP: %.1f\n", data.pdop);
    printf("HDOP: %.1f\n", data.hdop);
    printf("VDOP: %.1f\n", data.vdop);
  }
  else {
    printf("Error parsing GSA message: %d\n", result);
  }

  return 0;
}