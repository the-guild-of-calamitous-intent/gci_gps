////////////////////////////////////////////////
//  The MIT License (MIT)
//  Copyright (c) 2023 Kevin Walchko
//  see LICENSE for full details
////////////////////////////////////////////////
#include "gps.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to parse a GGA message manually
int parse_gga(const char *gga_msg, gga_t *data) {
  // Check if the message starts with $GPGGA or $GNGGA
  if (strncmp(gga_msg, "$GPGGA", 6) != 0 && strncmp(gga_msg, "$GNGGA", 6) != 0) {
    return -1; // Invalid GGA message
  }

  // Initialize data struct
  memset(data, 0, sizeof(gga_t));

  // Create a working copy of the input string
  char *buffer = strdup(gga_msg);
  if (!buffer) return -2; // Memory allocation failure

  int field_count = 0;
  char *start     = buffer;
  char *end       = buffer;

  // Manually parse fields
  while (*end != '\0' && *end != '*') { // Stop at checksum or end
    if (*end == ',') {
      *end = '\0'; // Null-terminate the field
      // Process the field based on field_count
      switch (field_count) {
      case 1: // UTC time
        strncpy(data->time, start, sizeof(data->time) - 1);
        data->time[sizeof(data->time) - 1] = '\0';
        break;
      case 2: // Latitude
        data->latitude = start[0] ? atof(start) : 0.0;
        break;
      case 3: // Latitude direction (N/S)
        data->lat_dir = start[0] ? start[0] : '\0';
        break;
      case 4: // Longitude
        data->longitude = start[0] ? atof(start) : 0.0;
        break;
      case 5: // Longitude direction (E/W)
        data->lon_dir = start[0] ? start[0] : '\0';
        break;
      case 6: // Fix quality
        data->fix_quality = start[0] ? atoi(start) : 0;
        break;
      case 7: // Number of satellites
        data->satellites = start[0] ? atoi(start) : 0;
        break;
      case 8: // HDOP
        // printf("hdop: %s\n", start);
        data->hdop = start[0] ? atof(start) : 0.0;
        break;
      case 9: // Altitude
        // printf("Alt: %s\n", start);
        data->altitude = start[0] ? atof(start) : 0.0;
        break;
      case 10: // Altitude unit
        data->alt_unit = start[0] ? start[0] : '\0';
        // printf("M: %c\n", start[0]);
        break;
      case 11: // Geoid separation
        // printf("geoid: %s\n", start);
        data->geoid_separation = start[0] ? atof(start) : 0.0;
        break;
      case 12: // Geoid unit
        // printf("geoid unit: %s\n", start);
        data->geoid_unit = start[0] ? start[0] : '\0';
        break;
      }
      field_count++;
      start = end + 1;
    }
    end++;
  }

  // Process the last field (geoid unit or empty)
  if (field_count == 12 && start[0]) {
    data->geoid_unit = start[0];
    field_count++;
  }

  free(buffer);

  // Check if enough fields were parsed
  if (field_count < 13) {
    return -3; // Incomplete GGA message
  }

  // Convert latitude and longitude from DDMM.MMMM to decimal degrees
  if (data->latitude != 0.0) {
    int degrees    = (int)(data->latitude / 100);
    double minutes = data->latitude - (degrees * 100);
    data->latitude = degrees + (minutes / 60.0);
    if (data->lat_dir == 'S') data->latitude = -data->latitude;
  }

  if (data->longitude != 0.0) {
    int degrees     = (int)(data->longitude / 100);
    double minutes  = data->longitude - (degrees * 100);
    data->longitude = degrees + (minutes / 60.0);
    if (data->lon_dir == 'W') data->longitude = -data->longitude;
  }

  return 0; // Success
}

// // Function to parse a GGA message
// int parse_gga(const char *gga_msg, GGAData *data) {
//   // Check if the message starts with $GPGGA or $GNGGA
//   if (strncmp(gga_msg, "$GPGGA", 6) != 0 && strncmp(gga_msg, "$GNGGA", 6) != 0) {
//     return -1; // Invalid GGA message
//   }

//   // Create a copy of the input string to avoid modifying it
//   char *buffer = strdup(gga_msg);
//   if (!buffer) return -2; // Memory allocation failure

//   // Tokenize the string by commas
//   char *token     = strtok(buffer, ",");
//   int field_count = 0;

//   while (token != NULL) {
//     switch (field_count) {
//     case 1: // UTC time
//       strncpy(data->time, token, sizeof(data->time) - 1);
//       data->time[sizeof(data->time) - 1] = '\0';
//       break;
//     case 2: // Latitude
//       data->latitude = atof(token);
//       break;
//     case 3: // Latitude direction (N/S)
//       data->lat_dir = token[0];
//       break;
//     case 4: // Longitude
//       data->longitude = atof(token);
//       break;
//     case 5: // Longitude direction (E/W)
//       data->lon_dir = token[0];
//       break;
//     case 6: // Fix quality
//       data->fix_quality = atoi(token);
//       break;
//     case 7: // Number of satellites
//       data->satellites = atoi(token);
//       break;
//     case 8: // HDOP
//       data->hdop = atof(token);
//       break;
//     case 9: // Altitude
//       data->altitude = atof(token);
//       break;
//     case 10: // Altitude unit
//       data->alt_unit = token[0];
//       break;
//     case 11: // Geoid separation
//       data->geoid_separation = atof(token);
//       break;
//     case 12: // Geoid unit
//       data->geoid_unit = token[0];
//       break;
//     }
//     token = strtok(NULL, ",");
//     field_count++;
//   }

//   free(buffer);

//   // Check if enough fields were parsed
//   if (field_count < 13) {
//     return -3; // Incomplete GGA message
//   }

//   // Convert latitude and longitude from DDMM.MMMM to decimal degrees
//   if (data->latitude != 0.0) {
//     int degrees    = (int)(data->latitude / 100);
//     double minutes = data->latitude - (degrees * 100);
//     data->latitude = degrees + (minutes / 60.0);
//     if (data->lat_dir == 'S') data->latitude = -data->latitude;
//   }

//   if (data->longitude != 0.0) {
//     int degrees     = (int)(data->longitude / 100);
//     double minutes  = data->longitude - (degrees * 100);
//     data->longitude = degrees + (minutes / 60.0);
//     if (data->lon_dir == 'W') data->longitude = -data->longitude;
//   }

//   return 0; // Success
// }
