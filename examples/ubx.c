#include "gci_gps.h"
#include <stdio.h>
#include <stdlib.h>

// Example: Simulate receiving UBX bytes from a UART
void process_ubx_data(uint8_t *data, size_t len) {
  ubx_parser_t parser;
  ubx_message_t msg;
  ubx_nav_pvt_t nav_pvt;

  ubx_parser_init(&parser);

  for (size_t i = 0; i < len; i++) {
    if (ubx_parser_process_byte(&parser, data[i], &msg)) {
      printf("Received valid UBX message: Class=0x%02X, ID=0x%02X, Length=%u\n",
             msg.msg_class, msg.msg_id, msg.payload_length);

      // Check for NAV-PVT message
      if (msg.msg_class == UBX_NAV_PVT_CLASS && msg.msg_id == UBX_NAV_PVT_ID) {
        if (ubx_parse_nav_pvt(&msg, &nav_pvt)) {
          printf("NAV-PVT Data:\n");
          printf("  iTOW: %u ms\n", nav_pvt.iTOW);
          printf("  Date: %04u-%02u-%02u %02u:%02u:%02u UTC\n",
                 nav_pvt.year, nav_pvt.month, nav_pvt.day,
                 nav_pvt.hour, nav_pvt.min, nav_pvt.sec);
          printf("  Fix Type: %u\n", nav_pvt.fixType);
          printf("  Latitude: %.7f deg\n", nav_pvt.lat / 1e7);
          printf("  Longitude: %.7f deg\n", nav_pvt.lon / 1e7);
          printf("  Height: %d mm\n", nav_pvt.height);
          printf("  Num Satellites: %u\n", nav_pvt.numSV);
        }
      }
      // Free payload memory
      free(msg.payload);
      msg.payload = NULL;
    }
  }
}

int main() {
  printf("start\n");
  // Example: Simulated NAV-PVT message (partial, for demonstration)
  uint8_t sample_data[] = {
      0xB5, 0x62, // Sync bytes
      0x01, 0x07, // Class=NAV, ID=PVT
      0x5C, 0x00, // Length=92 (little-endian)
      // Payload (example values, 92 bytes)
      0x00, 0x00, 0x00, 0x00, // iTOW
      0xE5, 0x07,             // Year=2025
      0x06, 0x1C,             // Month=6, Day=28
      0x16, 0x2B, 0x00,       // Hour=22, Min=43, Sec=0
      0x07,                   // Valid flags
      // ... (rest of payload omitted for brevity)
      // Checksum (example, not computed)
      0x12, 0x34 // CK_A, CK_B
  };

  process_ubx_data(sample_data, sizeof(sample_data));
  return 0;
}