#include "gci_gps.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static gps_msgs_t msgs = {
  .rmc_found = false,
  .gga_found = false,
  .gsa_found = false,
  .ublox_found = false
};


// Example usage
int main() {
  const char buffer[] = "$GNRMC,001031.00,A,4404.13993,N,12118.86023,W,0.146,,100117,,,A*7B\r\n$GPGSA,A,3,19,28,14,18,27,22,31,39,,,,,1.7,1.0,1.3*35\r\n$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";
  const uint8_t sample_ublox[] = {0xB5, 0x62,0x01, 0x07,0x5C,0x00,0x00,0x00,0x00,0x00,0xE5,0x07,0x06,0x1C,0x16, 0x2B, 0x00,0x07,0x12, 0x34};

  int num = process_gps_message(&msgs, buffer, sizeof(buffer));

  if (msgs.rmc_found) {
    printf("\n");
    rmc_t rmc;
    int result = parse_rmc((const char*)msgs.rmc, &rmc);
    if (result == 0) print_rmc(rmc);
    else printf("*** ERROR RMC ***\n");
  }

  if (msgs.gga_found) {
    printf("\n");
    gga_t gga;
    int result = parse_gga((const char*)msgs.gga, &gga);
    if (result == 0) print_gga(gga);
    else printf("*** ERROR GGA ***\n");
  }

  if (msgs.gsa_found) {
    printf("\n");
    gsa_t gsa;
    int result = parse_gsa(msgs.gsa, &gsa);
    if (result == 0) print_gsa(gsa);
    else printf("*** ERROR GSA ***\n");
  }

  return 0;
}