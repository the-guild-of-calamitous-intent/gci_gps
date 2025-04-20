////////////////////////////////////////////////
//  The MIT License (MIT)
//  Copyright (c) 2023 Kevin Walchko
//  see LICENSE for full details
////////////////////////////////////////////////
#include "gps.h"
#include <stdlib.h>
#include <string.h>

// Function to parse an RMC message manually
int parse_rmc(const char *rmc_msg, rmc_t *data) {
  // Check if the message starts with $GPRMC or $GNRMC
  if (strncmp(rmc_msg, "$GPRMC", 6) != 0 && strncmp(rmc_msg, "$GNRMC", 6) != 0) {
    return -1; // Invalid RMC message
  }

  // Initialize data struct
  memset(data, 0, sizeof(rmc_t));

  // Create a working copy of the input string
  char *buffer = strdup(rmc_msg);
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
      case 2: // Status
        data->status = start[0] ? start[0] : '\0';
        break;
      case 3: // Latitude
        data->latitude = start[0] ? atof(start) : 0.0;
        break;
      case 4: // Latitude direction (N/S)
        data->lat_dir = start[0] ? start[0] : '\0';
        break;
      case 5: // Longitude
        data->longitude = start[0] ? atof(start) : 0.0;
        break;
      case 6: // Longitude direction (E/W)
        data->lon_dir = start[0] ? start[0] : '\0';
        break;
      case 7: // Speed over ground (knots)
        data->speed = start[0] ? atof(start) : 0.0;
        break;
      case 8: // Course over ground
        data->course = start[0] ? atof(start) : 0.0;
        break;
      case 9: // Date
        strncpy(data->date, start, sizeof(data->date) - 1);
        data->date[sizeof(data->date) - 1] = '\0';
        break;
      case 10: // Magnetic variation
        data->mag_var = start[0] ? atof(start) : 0.0;
        break;
      case 11: // Magnetic variation direction
        data->mag_var_dir = start[0] ? start[0] : '\0';
        break;
      }
      field_count++;
      start = end + 1;
    }
    end++;
  }

  // Process the last field (mag_var_dir or empty)
  if (field_count == 11 && start[0]) {
    data->mag_var_dir = start[0];
    field_count++;
  }

  free(buffer);

  // Check if enough fields were parsed
  if (field_count < 12) {
    return -3; // Incomplete RMC message
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

  // Adjust magnetic variation sign based on direction
  if (data->mag_var != 0.0 && data->mag_var_dir == 'W') {
    data->mag_var = -data->mag_var;
  }

  return 0; // Success
}

// // Function to parse an RMC message
// int parse_rmc(const char *rmc_msg, RMCData *data) {
//   // Check if the message starts with $GPRMC or $GNRMC
//   if (strncmp(rmc_msg, "$GPRMC", 6) != 0 && strncmp(rmc_msg, "$GNRMC", 6) != 0) {
//     return -1; // Invalid RMC message
//   }

//   // Create a copy of the input string to avoid modifying it
//   char *buffer = strdup(rmc_msg);
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
//     case 2: // Status
//       data->status = token[0];
//       break;
//     case 3: // Latitude
//       data->latitude = atof(token);
//       break;
//     case 4: // Latitude direction (N/S)
//       data->lat_dir = token[0];
//       break;
//     case 5: // Longitude
//       data->longitude = atof(token);
//       break;
//     case 6: // Longitude direction (E/W)
//       data->lon_dir = token[0];
//       break;
//     case 7: // Speed over ground (knots)
//       data->speed = atof(token);
//       break;
//     case 8: // Course over ground
//       data->course = atof(token);
//       break;
//     case 9: // Date
//       strncpy(data->date, token, sizeof(data->date) - 1);
//       data->date[sizeof(data->date) - 1] = '\0';
//       break;
//     case 10: // Magnetic variation
//       data->mag_var = atof(token);
//       break;
//     case 11: // Magnetic variation direction
//       data->mag_var_dir = token[0];
//       break;
//     }
//     token = strtok(NULL, ",");
//     field_count++;
//   }

//   free(buffer);

//   // Check if enough fields were parsed
//   if (field_count < 12) {
//     return -3; // Incomplete RMC message
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

//   // Adjust magnetic variation sign based on direction
//   if (data->mag_var != 0.0 && data->mag_var_dir == 'W') {
//     data->mag_var = -data->mag_var;
//   }

//   return 0; // Success
// }
