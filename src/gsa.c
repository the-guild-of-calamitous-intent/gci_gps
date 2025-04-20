////////////////////////////////////////////////
//  The MIT License (MIT)
//  Copyright (c) 2023 Kevin Walchko
//  see LICENSE for full details
////////////////////////////////////////////////
#include "gps.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to parse a GSA message manually
int parse_gsa(const char *gsa_msg, gsa_t *data) {
  // Check if the message starts with $GPGSA or $GNGSA
  if (strncmp(gsa_msg, "$GPGSA", 6) != 0 && strncmp(gsa_msg, "$GNGSA", 6) != 0) {
    return -1; // Invalid GSA message
  }

  // printf("msg: %s\n", gsa_msg);

  // Initialize data struct
  memset(data, 0, sizeof(gsa_t));

  // Create a working copy of the input string
  char *buffer = strdup(gsa_msg);
  if (!buffer) return -2; // Memory allocation failure

  int field_count = 0;
  int sat_index   = 0;
  char *start     = buffer;
  char *end       = buffer;

  // Manually parse fields
  while (*end != '\0' && *end != '*') { // Stop at checksum or end
    if (*end == ',') {
      *end = '\0'; // Null-terminate the field
      // Process the field based on field_count
      switch (field_count) {
      case 1: // Mode
        data->mode = start[0] ? start[0] : '\0';
        break;
      case 2: // Fix type
        data->fix_type = start[0] ? atoi(start) : 0;
        break;
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
      case 9:
      case 10:
      case 11:
      case 12:
      case 13:
      case 14:
        // Satellite PRN numbers (up to 12)
        data->satellites[sat_index++] = start[0] ? atoi(start) : 0;
        break;
      case 15: // PDOP
        // printf("pdop: %s\n", start);
        data->pdop = start[0] ? atof(start) : 0.0;
        break;
      case 16: // HDOP
        // printf("hdop: %s\n", start);
        data->hdop = start[0] ? atof(start) : 0.0;
        break;
      case 17: // VDOP
        // printf("vdop: %s\n", start);
        data->vdop = start[0] ? atof(start) : 0.0;
        break;
      }
      field_count++;
      start = end + 1;
    }
    end++;
  }

  // Process the last field (VDOP, if not already processed)
  if (field_count == 17 && start[0]) {
    data->vdop = atof(start);
    field_count++;
  }

  free(buffer);

  // Check if enough fields were parsed
  if (field_count < 18) {
    return -3; // Incomplete GSA message
  }

  return 0; // Success
}

// // Function to parse a GSA message
// int parse_gsa(const char *gsa_msg, GSAData *data) {
//     // Check if the message starts with $GPGSA or $GNGSA
//     if (strncmp(gsa_msg, "$GPGSA", 6) != 0 && strncmp(gsa_msg, "$GNGSA", 6) != 0) {
//         return -1; // Invalid GSA message
//     }
//     printf("msg: %s\n", gsa_msg);

//     // Create a copy of the input string to avoid modifying it
//     char *buffer = strdup(gsa_msg);
//     if (!buffer) return -2; // Memory allocation failure

//     // Tokenize the string by commas
//     char *token = strtok(buffer, ",");
//     int field_count = 0;
//     int sat_index = 0;

//     while (token != NULL) {
//         switch (field_count) {
//             case 1: // Mode
//                 data->mode = token[0];
//                 break;
//             case 2: // Fix type
//                 data->fix_type = atoi(token);
//                 break;
//             case 3: case 4: case 5: case 6: case 7: case 8:
//             case 9: case 10: case 11: case 12: case 13: case 14:
//                 // Satellite PRN numbers (up to 12)
//                 data->satellites[sat_index++] = token[0] ? atoi(token) : 0;
//                 break;
//             case 15: // PDOP
//                 data->pdop = atof(token);
//                 break;
//             case 16: // HDOP
//                 data->hdop = atof(token);
//                 break;
//             case 17: // VDOP
//                 data->vdop = atof(token);
//                 break;
//         }
//         token = strtok(NULL, ",");
//         printf("token[%d] %s\n", field_count, token);
//         field_count++;
//     }

//     free(buffer);

//     // Check if enough fields were parsed
//     // if (field_count < 18) {
//     //     printf("field_count: %d\n", (int)field_count);
//     //     return -3; // Incomplete GSA message
//     // }

//     return 0; // Success
// }

// // Example usage
// int main() {
//     const char *sample_gsa = "$GPGSA,A,3,19,28,14,18,27,22,31,39,,,,,1.7,1.0,1.3*35";
//     GSAData data = {0}; // Initialize struct

//     int result = parse_gsa(sample_gsa, &data);
//     if (result == 0) {
//         printf("Mode: %c\n", data.mode);
//         printf("Fix Type: %d\n", data.fix_type);
//         printf("Satellites Used: ");
//         for (int i = 0; i < 12 && data.satellites[i] != 0; i++) {
//             printf("%d ", data.satellites[i]);
//         }
//         printf("\n");
//         printf("PDOP: %.1f\n", data.pdop);
//         printf("HDOP: %.1f\n", data.hdop);
//         printf("VDOP: %.1f\n", data.vdop);
//     } else {
//         printf("Error parsing GSA message: %d\n", result);
//     }

//     return 0;
// }
